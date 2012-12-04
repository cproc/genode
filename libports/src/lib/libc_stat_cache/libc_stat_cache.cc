/*
 * \brief  libc stat() cache
 * \author Christian Prochaska
 * \date   2012-12-04
 */


#include <base/env.h>
#include <base/printf.h>
#include <os/path.h>
#include <util/list.h>
#include <util/token.h>

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>


using namespace Genode;

static bool verbose = false;

typedef Genode::Path<PATH_MAX> Absolute_path;


struct Stat_cache_entry
{
	int result;
	int errno_;
	struct stat *st;

	Stat_cache_entry(int result, int errno_, struct stat *st)
	: result(result), errno_(errno_), st(st) { }
};


/**
 * path element token
 */

struct Scanner_policy_path_element
{
	static bool identifier_char(char c, unsigned /* i */)
	{
		return (c != '/') && (c != 0);
	}
};


typedef Genode::Token<Scanner_policy_path_element> Path_element_token;

/*
 * TODO: make this class reusable for the tar indexer and other similar use cases
 * - use template for 'Stat_cache_entry' type
 */
struct Node : List<Node>, List<Node>::Element
{
	char const *name;
	Stat_cache_entry *value;

	Node(char const *name, Stat_cache_entry *value) : name(name), value(value) { }

	Node *lookup(char const *name)
	{
		Absolute_path lookup_path(name);

		if (verbose)
			PDBG("lookup_path = %s", lookup_path.base());

		Node *parent_node = this;
		Node *child_node;

		Path_element_token t(lookup_path.base());

		while (t) {

			if (t.type() != Path_element_token::IDENT) {
					t = t.next();
					continue;
			}

			char path_element[PATH_MAX];

			t.string(path_element, sizeof(path_element));

			if (verbose)
				PDBG("path_element = %s", path_element);

			for (child_node = parent_node->first(); child_node; child_node = child_node->next()) {
				if (verbose)
					PDBG("comparing with node %s", child_node->name);
				if (strcmp(child_node->name, path_element) == 0) {
					if (verbose)
						PDBG("found matching child node");
					parent_node = child_node;
					break;
				}
			}

			if (!child_node)
				return 0;

			t = t.next();
		}

		return parent_node;
	}


	Node *lookup_child(int index)
	{
		for (Node *child_node = first(); child_node; child_node = child_node->next(), index--) {
			if (index == 0)
				return child_node;
		}

		return 0;
	}


	size_t num_dirent()
	{
		size_t count = 0;
		for (Node *child_node = first(); child_node; child_node = child_node->next(), count++) ;
		return count;
	}

};


static struct Node _root_node("/", 0);

/*
 *  TODO: make this function reusable for the tar indexer and other possible use cases
 */
static void add_to_cache(const char *path, Stat_cache_entry *value)
{
	Absolute_path current_path(path);

	if (verbose)
		PDBG("current_path = %s", current_path.base());

	char path_element[PATH_MAX];

	Path_element_token t(current_path.base());

	Node *parent_node = &_root_node;
	Node *child_node;

	while(t) {

		if (t.type() != Path_element_token::IDENT) {
				t = t.next();
				continue;
		}

		Absolute_path remaining_path(t.start());

		t.string(path_element, sizeof(path_element));

		for (child_node = parent_node->first(); child_node; child_node = child_node->next()) {
			if (strcmp(child_node->name, path_element) == 0)
				break;
		}

		if (child_node) {

			if (verbose)
				PDBG("found node for %s", path_element);

			if (remaining_path.has_single_element()) {
				/* Found a node for the record to be inserted.
				 * This is usually a directory node without
				 * record. */
				child_node->value = value;
			}
		} else {
			if (remaining_path.has_single_element()) {

				if (verbose)
					PDBG("creating node for %s", path_element);

				/*
				 * TODO: find 'path_element' in 'record->name'
				 * and use the location in the record as name
				 * pointer to save some memory
				 */
				size_t name_size = strlen(path_element) + 1;
				char *name = (char*)env()->heap()->alloc(name_size);
				strncpy(name, path_element, name_size);
				child_node = new (env()->heap()) Node(name, value);
			} else {

				if (verbose)
					PDBG("creating node without record for %s", path_element);

				/* create a directory node without record */
				size_t name_size = strlen(path_element) + 1;
				char *name = (char*)env()->heap()->alloc(name_size);
				strncpy(name, path_element, name_size);
				child_node = new (env()->heap()) Node(name, 0);
			}
			parent_node->insert(child_node);
		}

		parent_node = child_node;
		t = t.next();
	}
}


int stat_cache_lookup(const char *path, struct stat *buf)
{
	if (verbose)
		PDBG("looking up cache entry for %s", path);

	Node *node = _root_node.lookup(path);

	if (!node)
		return -2; /* no cache entry */

	Stat_cache_entry *value = node->value;

	if (!value)
		return -2; /* no cache entry */

	if (verbose)
		PDBG("found cache entry for %s", path);

	if (value->result == -1)
		errno = value->errno_;

	*buf = *(value->st);
	return value->result;
}


void stat_cache_insert(const char *path, int result, int errno_, struct stat const &buf)
{
	if (verbose)
		PDBG("adding cache entry for %s", path);
	struct stat *st = new (env()->heap()) struct stat(buf);
	Stat_cache_entry *stat_cache_entry = new (env()->heap()) Stat_cache_entry(result, errno_, st);
	add_to_cache(path, stat_cache_entry);
}
