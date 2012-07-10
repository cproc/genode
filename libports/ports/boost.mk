BOOST     = boost_1_50_0
BOOST_TBZ = $(BOOST).tar.bz2
BOOST_URL = http://sourceforge.net/projects/boost/files/boost/1.50.0/$(BOOST_TBZ)

#
# Interface to top-level prepare Makefile
#
PORTS += boost

prepare-boost: $(CONTRIB_DIR)/$(BOOST) include/boost

#
# Port-specific local rules
#
$(DOWNLOAD_DIR)/$(BOOST_TBZ):
	$(VERBOSE)wget -c -P $(DOWNLOAD_DIR) $(BOOST_URL) && touch $@

$(CONTRIB_DIR)/$(BOOST): $(DOWNLOAD_DIR)/$(BOOST_TBZ)
	$(VERBOSE)tar xjf $< -C $(CONTRIB_DIR) && touch $@

include/boost:
	$(VERBOSE)ln -s ../$(CONTRIB_DIR)/$(BOOST)/boost $@

clean-boost:
	$(VERBOSE)rm -f include/boost
