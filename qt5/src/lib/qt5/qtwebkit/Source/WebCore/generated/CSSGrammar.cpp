/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse         cssyyparse
#define yylex           cssyylex
#define yyerror         cssyyerror
#define yylval          cssyylval
#define yychar          cssyychar
#define yydebug         cssyydebug
#define yynerrs         cssyynerrs


/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "generated/CSSGrammar.y"


/*
 *  Copyright (C) 2002-2003 Lars Knoll (knoll@kde.org)
 *  Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights reserved.
 *  Copyright (C) 2006 Alexey Proskuryakov (ap@nypop.com)
 *  Copyright (C) 2008 Eric Seidel <eric@webkit.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "config.h"

#include "CSSParser.h"
#include "CSSParserMode.h"
#include "CSSPrimitiveValue.h"
#include "CSSPropertyNames.h"
#include "CSSSelector.h"
#include "CSSSelectorList.h"
#include "Document.h"
#include "HTMLNames.h"
#include "MediaList.h"
#include "MediaQueryExp.h"
#include "StyleRule.h"
#include "StyleSheetContents.h"
#include "WebKitCSSKeyframeRule.h"
#include "WebKitCSSKeyframesRule.h"
#include <wtf/FastMalloc.h>
#include <stdlib.h>
#include <string.h>

using namespace WebCore;
using namespace HTMLNames;

#define YYMALLOC fastMalloc
#define YYFREE fastFree

#define YYENABLE_NLS 0
#define YYLTYPE_IS_TRIVIAL 1
#define YYMAXDEPTH 10000
#define YYDEBUG 0

#if YYDEBUG > 0
#define YYPRINT(File,Type,Value) print_token_value(File,Type,Value)
static void print_token_value(FILE * yyoutput, int yytype, YYSTYPE const &yyvalue)
{
    switch (yytype) {
    case IDENT:
    case STRING:
    case NTH:
    case HEX:
    case IDSEL:
    case DIMEN:
    case INVALIDDIMEN:
    case URI:
    case FUNCTION:
    case ANYFUNCTION:
    case NOTFUNCTION:
    case CALCFUNCTION:
    case MINFUNCTION:
    case MAXFUNCTION:
    case VAR_DEFINITION:
    case UNICODERANGE:
        YYFPRINTF(yyoutput, "%s", String(yyvalue.string).utf8().data());
        break;
    default:
        break;
    }
}
#endif



/* Line 268 of yacc.c  */
#line 166 "generated/CSSGrammar.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOKEN_EOF = 0,
     LOWEST_PREC = 258,
     UNIMPORTANT_TOK = 259,
     WHITESPACE = 260,
     SGML_CD = 261,
     INCLUDES = 262,
     DASHMATCH = 263,
     BEGINSWITH = 264,
     ENDSWITH = 265,
     CONTAINS = 266,
     STRING = 267,
     IDENT = 268,
     NTH = 269,
     HEX = 270,
     IDSEL = 271,
     IMPORT_SYM = 272,
     PAGE_SYM = 273,
     MEDIA_SYM = 274,
     FONT_FACE_SYM = 275,
     CHARSET_SYM = 276,
     NAMESPACE_SYM = 277,
     VARFUNCTION = 278,
     WEBKIT_RULE_SYM = 279,
     WEBKIT_DECLS_SYM = 280,
     WEBKIT_KEYFRAME_RULE_SYM = 281,
     WEBKIT_KEYFRAMES_SYM = 282,
     WEBKIT_VALUE_SYM = 283,
     WEBKIT_MEDIAQUERY_SYM = 284,
     WEBKIT_SELECTOR_SYM = 285,
     WEBKIT_REGION_RULE_SYM = 286,
     WEBKIT_VIEWPORT_RULE_SYM = 287,
     TOPLEFTCORNER_SYM = 288,
     TOPLEFT_SYM = 289,
     TOPCENTER_SYM = 290,
     TOPRIGHT_SYM = 291,
     TOPRIGHTCORNER_SYM = 292,
     BOTTOMLEFTCORNER_SYM = 293,
     BOTTOMLEFT_SYM = 294,
     BOTTOMCENTER_SYM = 295,
     BOTTOMRIGHT_SYM = 296,
     BOTTOMRIGHTCORNER_SYM = 297,
     LEFTTOP_SYM = 298,
     LEFTMIDDLE_SYM = 299,
     LEFTBOTTOM_SYM = 300,
     RIGHTTOP_SYM = 301,
     RIGHTMIDDLE_SYM = 302,
     RIGHTBOTTOM_SYM = 303,
     ATKEYWORD = 304,
     IMPORTANT_SYM = 305,
     MEDIA_ONLY = 306,
     MEDIA_NOT = 307,
     MEDIA_AND = 308,
     REMS = 309,
     QEMS = 310,
     EMS = 311,
     EXS = 312,
     PXS = 313,
     CMS = 314,
     MMS = 315,
     INS = 316,
     PTS = 317,
     PCS = 318,
     DEGS = 319,
     RADS = 320,
     GRADS = 321,
     TURNS = 322,
     MSECS = 323,
     SECS = 324,
     HERTZ = 325,
     KHERTZ = 326,
     DIMEN = 327,
     INVALIDDIMEN = 328,
     PERCENTAGE = 329,
     FLOATTOKEN = 330,
     INTEGER = 331,
     VW = 332,
     VH = 333,
     VMIN = 334,
     DPPX = 335,
     DPI = 336,
     DPCM = 337,
     URI = 338,
     FUNCTION = 339,
     ANYFUNCTION = 340,
     NOTFUNCTION = 341,
     CALCFUNCTION = 342,
     MINFUNCTION = 343,
     MAXFUNCTION = 344,
     VAR_DEFINITION = 345,
     UNICODERANGE = 346
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 114 "generated/CSSGrammar.y"

bool boolean;
char character;
int integer;
double number;
CSSParserString string;

StyleRuleBase* rule;
Vector<RefPtr<StyleRuleBase> >* ruleList;
CSSParserSelector* selector;
Vector<OwnPtr<CSSParserSelector> >* selectorList;
CSSSelector::MarginBoxType marginBox;
CSSSelector::Relation relation;
MediaQuerySet* mediaList;
MediaQuery* mediaQuery;
MediaQuery::Restrictor mediaQueryRestrictor;
MediaQueryExp* mediaQueryExp;
CSSParserValue value;
CSSParserValueList* valueList;
Vector<OwnPtr<MediaQueryExp> >* mediaQueryExpList;
StyleKeyframe* keyframe;
Vector<RefPtr<StyleKeyframe> >* keyframeRuleList;
float val;
CSSPropertyID id;



/* Line 293 of yacc.c  */
#line 322 "generated/CSSGrammar.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 343 of yacc.c  */
#line 140 "generated/CSSGrammar.y"


static inline int cssyyerror(void*, const char*)
{
return 1;
}



/* Line 343 of yacc.c  */
#line 344 "generated/CSSGrammar.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  21
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1953

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  112
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  105
/* YYNRULES -- Number of rules.  */
#define YYNRULES  315
/* YYNRULES -- Number of states.  */
#define YYNSTATES  603

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   346

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,   110,     2,   111,     2,     2,
     100,   101,    20,   103,   102,   106,    18,   109,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    17,    99,
       2,   108,   105,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    19,     2,   107,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    97,    21,    98,   104,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     8,    11,    14,    17,    20,    23,    26,
      33,    40,    46,    52,    58,    64,    65,    68,    69,    72,
      75,    76,    78,    80,    82,    88,    92,    96,   102,   106,
     107,   111,   113,   115,   117,   119,   121,   123,   125,   127,
     129,   131,   133,   135,   136,   140,   142,   144,   146,   148,
     150,   152,   154,   156,   158,   160,   162,   163,   171,   179,
     187,   192,   197,   204,   211,   215,   219,   220,   223,   225,
     227,   230,   231,   236,   246,   248,   254,   255,   259,   260,
     262,   264,   266,   271,   272,   274,   276,   281,   284,   285,
     286,   288,   299,   308,   313,   316,   317,   328,   330,   332,
     333,   337,   344,   346,   352,   355,   357,   358,   369,   374,
     379,   381,   384,   386,   387,   389,   394,   395,   403,   405,
     407,   409,   411,   413,   415,   417,   419,   421,   423,   425,
     427,   429,   431,   433,   435,   436,   445,   450,   455,   457,
     458,   469,   472,   475,   478,   480,   481,   483,   485,   487,
     488,   489,   498,   500,   505,   508,   511,   513,   515,   518,
     522,   525,   527,   530,   533,   535,   538,   540,   543,   547,
     550,   552,   558,   561,   563,   565,   567,   570,   573,   575,
     577,   579,   581,   583,   586,   589,   594,   603,   609,   619,
     621,   623,   625,   627,   629,   631,   633,   635,   638,   641,
     645,   652,   659,   667,   674,   681,   683,   686,   688,   692,
     694,   697,   700,   704,   708,   713,   717,   723,   728,   733,
     740,   746,   752,   755,   762,   769,   772,   776,   781,   784,
     787,   790,   791,   793,   797,   800,   804,   807,   810,   813,
     814,   816,   819,   822,   825,   828,   832,   835,   838,   841,
     844,   850,   852,   854,   856,   859,   862,   865,   868,   871,
     874,   877,   880,   883,   886,   889,   892,   895,   898,   901,
     904,   907,   910,   913,   916,   919,   922,   925,   928,   931,
     934,   937,   940,   946,   951,   956,   960,   962,   968,   971,
     974,   977,   980,   983,   990,   993,   997,  1001,  1003,  1006,
    1008,  1013,  1019,  1023,  1025,  1027,  1033,  1037,  1039,  1042,
    1046,  1050,  1053,  1059,  1063,  1065
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     113,     0,    -1,   120,   122,   121,   126,    -1,   114,   120,
      -1,   116,   120,    -1,   117,   120,    -1,   118,   120,    -1,
     119,   120,    -1,   115,   120,    -1,    29,    97,   120,   127,
     120,    98,    -1,    31,    97,   120,   156,   120,    98,    -1,
      30,    97,   174,   194,    98,    -1,    33,    97,   120,   199,
      98,    -1,    34,     5,   120,   144,    98,    -1,    35,    97,
     120,   178,    98,    -1,    -1,   120,     5,    -1,    -1,   121,
       6,    -1,   121,     5,    -1,    -1,   124,    -1,    98,    -1,
       0,    -1,    26,   120,    12,   120,    99,    -1,    26,     1,
     215,    -1,    26,     1,    99,    -1,    26,   120,    12,   120,
      99,    -1,    26,   120,    99,    -1,    -1,   126,   128,   121,
      -1,   177,    -1,   150,    -1,   160,    -1,   167,    -1,   153,
      -1,   135,    -1,   134,    -1,   170,    -1,   127,    -1,   125,
      -1,   214,    -1,   213,    -1,    -1,   129,   131,   121,    -1,
     177,    -1,   160,    -1,   167,    -1,   153,    -1,   130,    -1,
     214,    -1,   213,    -1,   135,    -1,   134,    -1,   150,    -1,
     120,    -1,    -1,   133,    22,   132,   137,   120,   145,    99,
      -1,   133,    22,   132,   137,   120,   145,     0,    -1,   133,
      22,   132,   137,   120,   145,   215,    -1,   133,    22,     1,
      99,    -1,   133,    22,     1,   215,    -1,    27,   120,   136,
     137,   120,    99,    -1,    27,   120,   136,   137,   120,   215,
      -1,    27,     1,   215,    -1,    27,     1,    99,    -1,    -1,
      13,   120,    -1,    12,    -1,    88,    -1,    13,   120,    -1,
      -1,    17,   120,   199,   120,    -1,   143,   120,   100,   120,
     138,   120,   139,   101,   120,    -1,   140,    -1,   141,   120,
      58,   120,   140,    -1,    -1,    58,   120,   141,    -1,    -1,
      56,    -1,    57,    -1,   141,    -1,   143,   120,   151,   142,
      -1,    -1,   146,    -1,   144,    -1,   146,   102,   120,   144,
      -1,   146,     1,    -1,    -1,    -1,   120,    -1,   148,    24,
     120,   146,   176,    97,   147,   120,   129,   212,    -1,   148,
      24,   149,    97,   147,   120,   129,   212,    -1,   148,    24,
     149,    99,    -1,    13,   120,    -1,    -1,   152,    32,   120,
     154,   149,    97,   147,   120,   155,   123,    -1,    13,    -1,
      12,    -1,    -1,   155,   156,   120,    -1,   157,   120,    97,
     120,   194,    98,    -1,   158,    -1,   157,   120,   102,   120,
     158,    -1,   172,    79,    -1,    13,    -1,    -1,   159,    23,
     120,   161,   149,    97,   147,   174,   162,   123,    -1,   159,
      23,     1,   215,    -1,   159,    23,     1,    99,    -1,    13,
      -1,    13,   192,    -1,   192,    -1,    -1,   194,    -1,   162,
     163,   120,   194,    -1,    -1,   165,   164,   120,    97,   120,
     194,   123,    -1,    38,    -1,    39,    -1,    40,    -1,    41,
      -1,    42,    -1,    43,    -1,    44,    -1,    45,    -1,    46,
      -1,    47,    -1,    48,    -1,    49,    -1,    50,    -1,    51,
      -1,    52,    -1,    53,    -1,    -1,   166,    25,   149,    97,
     147,   174,   194,   123,    -1,   166,    25,     1,   215,    -1,
     166,    25,     1,    99,    -1,   178,    -1,    -1,   169,    36,
       5,   168,   176,    97,   147,   120,   129,   212,    -1,   103,
     120,    -1,   104,   120,    -1,   105,   120,    -1,   173,    -1,
      -1,   106,    -1,   103,    -1,   120,    -1,    -1,    -1,   175,
     178,   176,    97,   147,   174,   194,   123,    -1,   180,    -1,
     178,   102,   120,   180,    -1,   178,     1,    -1,   180,     5,
      -1,   182,    -1,   179,    -1,   179,   182,    -1,   180,   171,
     182,    -1,   180,     1,    -1,    21,    -1,    20,    21,    -1,
      13,    21,    -1,   184,    -1,   184,   185,    -1,   185,    -1,
     181,   184,    -1,   181,   184,   185,    -1,   181,   185,    -1,
     182,    -1,   183,   120,   102,   120,   182,    -1,   183,     1,
      -1,    13,    -1,    20,    -1,   186,    -1,   185,   186,    -1,
     185,     1,    -1,    16,    -1,    15,    -1,   187,    -1,   189,
      -1,   193,    -1,    18,    13,    -1,    13,   120,    -1,    19,
     120,   188,   107,    -1,    19,   120,   188,   190,   120,   191,
     120,   107,    -1,    19,   120,   181,   188,   107,    -1,    19,
     120,   181,   188,   190,   120,   191,   120,   107,    -1,   108,
      -1,     7,    -1,     8,    -1,     9,    -1,    10,    -1,    11,
      -1,    13,    -1,    12,    -1,    17,    13,    -1,    17,    13,
      -1,    17,    17,    13,    -1,    17,    90,   120,   183,   120,
     101,    -1,    17,    89,   120,    14,   120,   101,    -1,    17,
      89,   120,   172,    81,   120,   101,    -1,    17,    89,   120,
      13,   120,   101,    -1,    17,    91,   120,   182,   120,   101,
      -1,   196,    -1,   195,   196,    -1,   195,    -1,     1,   216,
       1,    -1,     1,    -1,   195,     1,    -1,   195,   216,    -1,
     196,    99,   120,    -1,   196,   216,   120,    -1,   196,   216,
      99,   120,    -1,     1,    99,   120,    -1,     1,   216,     1,
      99,   120,    -1,   195,   196,    99,   120,    -1,   195,     1,
      99,   120,    -1,   195,     1,   216,     1,    99,   120,    -1,
      95,    17,   120,   199,   198,    -1,   197,    17,   120,   199,
     198,    -1,   197,     1,    -1,   197,    17,   120,     1,   199,
     198,    -1,   197,    17,   120,   199,   198,     1,    -1,    55,
     120,    -1,   197,    17,   120,    -1,   197,    17,   120,     1,
      -1,   197,   215,    -1,    13,   120,    -1,    55,   120,    -1,
      -1,   201,    -1,   199,   200,   201,    -1,   199,   216,    -1,
     199,   216,     1,    -1,   199,     1,    -1,   109,   120,    -1,
     102,   120,    -1,    -1,   202,    -1,   173,   202,    -1,    12,
     120,    -1,    13,   120,    -1,    77,   120,    -1,   173,    77,
     120,    -1,    88,   120,    -1,    96,   120,    -1,    15,   120,
      -1,   110,   120,    -1,    28,   120,    13,   101,   120,    -1,
     203,    -1,   209,    -1,   211,    -1,   111,   120,    -1,    81,
     120,    -1,    80,   120,    -1,    79,   120,    -1,    63,   120,
      -1,    64,   120,    -1,    65,   120,    -1,    66,   120,    -1,
      67,   120,    -1,    68,   120,    -1,    69,   120,    -1,    70,
     120,    -1,    71,   120,    -1,    72,   120,    -1,    73,   120,
      -1,    74,   120,    -1,    75,   120,    -1,    76,   120,    -1,
      61,   120,    -1,    60,   120,    -1,    62,   120,    -1,    59,
     120,    -1,    82,   120,    -1,    83,   120,    -1,    84,   120,
      -1,    85,   120,    -1,    86,   120,    -1,    87,   120,    -1,
      89,   120,   199,   101,   120,    -1,    89,   120,   199,     0,
      -1,    89,   120,   101,   120,    -1,    89,   120,     1,    -1,
     202,    -1,    28,   120,    13,   101,   120,    -1,   173,   202,
      -1,   103,     5,    -1,   106,     5,    -1,    20,   120,    -1,
     109,   120,    -1,   100,   120,   207,   120,   101,   120,    -1,
     204,   120,    -1,   207,   205,   204,    -1,   207,   205,   206,
      -1,   206,    -1,   207,     1,    -1,   207,    -1,   208,   102,
     120,   207,    -1,    92,   120,   207,   101,   120,    -1,    92,
     120,     1,    -1,    93,    -1,    94,    -1,   210,   120,   208,
     101,   120,    -1,   210,   120,     1,    -1,   123,    -1,     1,
     123,    -1,    54,     1,   215,    -1,    54,     1,    99,    -1,
       1,   215,    -1,    97,     1,   216,     1,   123,    -1,    97,
       1,   123,    -1,   215,    -1,   216,     1,   215,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   381,   381,   382,   383,   384,   385,   386,   387,   391,
     397,   403,   409,   421,   427,   436,   437,   440,   442,   443,
     446,   448,   453,   454,   458,   465,   467,   472,   476,   481,
     483,   490,   491,   492,   493,   494,   495,   496,   497,   510,
     513,   514,   515,   519,   520,   531,   532,   533,   534,   541,
     542,   543,   544,   545,   546,   553,   560,   566,   569,   572,
     576,   580,   587,   591,   594,   597,   603,   604,   608,   609,
     613,   619,   622,   628,   641,   645,   652,   655,   661,   664,
     667,   673,   677,   684,   687,   691,   696,   703,   709,   715,
     721,   727,   730,   733,   740,   814,   820,   826,   827,   831,
     832,   840,   846,   850,   858,   859,   872,   878,   890,   894,
     901,   906,   913,   918,   925,   926,   930,   930,   938,   941,
     944,   947,   950,   953,   956,   959,   962,   965,   968,   971,
     974,   977,   980,   983,   989,   995,   999,  1003,  1056,  1067,
    1073,  1084,  1085,  1086,  1090,  1091,  1095,  1096,  1100,  1106,
    1112,  1118,  1124,  1132,  1140,  1146,  1152,  1155,  1159,  1172,
    1184,  1190,  1191,  1192,  1196,  1200,  1205,  1210,  1214,  1219,
    1227,  1234,  1241,  1247,  1253,  1260,  1263,  1269,  1275,  1282,
    1293,  1294,  1295,  1299,  1309,  1318,  1323,  1329,  1334,  1343,
    1346,  1349,  1352,  1355,  1358,  1364,  1365,  1369,  1380,  1389,
    1404,  1418,  1428,  1438,  1456,  1474,  1477,  1482,  1485,  1488,
    1491,  1494,  1500,  1504,  1507,  1510,  1514,  1517,  1523,  1527,
    1534,  1544,  1560,  1564,  1572,  1578,  1583,  1589,  1595,  1602,
    1608,  1609,  1613,  1617,  1630,  1633,  1636,  1642,  1645,  1648,
    1654,  1655,  1656,  1657,  1663,  1664,  1665,  1666,  1667,  1668,
    1669,  1677,  1680,  1683,  1686,  1692,  1693,  1694,  1695,  1696,
    1697,  1698,  1699,  1700,  1701,  1702,  1703,  1704,  1705,  1706,
    1707,  1708,  1709,  1710,  1711,  1712,  1719,  1720,  1721,  1722,
    1723,  1724,  1728,  1736,  1744,  1753,  1764,  1765,  1772,  1776,
    1779,  1782,  1785,  1791,  1806,  1810,  1823,  1835,  1836,  1842,
    1845,  1860,  1868,  1875,  1878,  1884,  1892,  1900,  1903,  1909,
    1912,  1918,  1936,  1939,  1945,  1946
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "TOKEN_EOF", "error", "$undefined", "LOWEST_PREC", "UNIMPORTANT_TOK",
  "WHITESPACE", "SGML_CD", "INCLUDES", "DASHMATCH", "BEGINSWITH",
  "ENDSWITH", "CONTAINS", "STRING", "IDENT", "NTH", "HEX", "IDSEL", "':'",
  "'.'", "'['", "'*'", "'|'", "IMPORT_SYM", "PAGE_SYM", "MEDIA_SYM",
  "FONT_FACE_SYM", "CHARSET_SYM", "NAMESPACE_SYM", "VARFUNCTION",
  "WEBKIT_RULE_SYM", "WEBKIT_DECLS_SYM", "WEBKIT_KEYFRAME_RULE_SYM",
  "WEBKIT_KEYFRAMES_SYM", "WEBKIT_VALUE_SYM", "WEBKIT_MEDIAQUERY_SYM",
  "WEBKIT_SELECTOR_SYM", "WEBKIT_REGION_RULE_SYM",
  "WEBKIT_VIEWPORT_RULE_SYM", "TOPLEFTCORNER_SYM", "TOPLEFT_SYM",
  "TOPCENTER_SYM", "TOPRIGHT_SYM", "TOPRIGHTCORNER_SYM",
  "BOTTOMLEFTCORNER_SYM", "BOTTOMLEFT_SYM", "BOTTOMCENTER_SYM",
  "BOTTOMRIGHT_SYM", "BOTTOMRIGHTCORNER_SYM", "LEFTTOP_SYM",
  "LEFTMIDDLE_SYM", "LEFTBOTTOM_SYM", "RIGHTTOP_SYM", "RIGHTMIDDLE_SYM",
  "RIGHTBOTTOM_SYM", "ATKEYWORD", "IMPORTANT_SYM", "MEDIA_ONLY",
  "MEDIA_NOT", "MEDIA_AND", "REMS", "QEMS", "EMS", "EXS", "PXS", "CMS",
  "MMS", "INS", "PTS", "PCS", "DEGS", "RADS", "GRADS", "TURNS", "MSECS",
  "SECS", "HERTZ", "KHERTZ", "DIMEN", "INVALIDDIMEN", "PERCENTAGE",
  "FLOATTOKEN", "INTEGER", "VW", "VH", "VMIN", "DPPX", "DPI", "DPCM",
  "URI", "FUNCTION", "ANYFUNCTION", "NOTFUNCTION", "CALCFUNCTION",
  "MINFUNCTION", "MAXFUNCTION", "VAR_DEFINITION", "UNICODERANGE", "'{'",
  "'}'", "';'", "'('", "')'", "','", "'+'", "'~'", "'>'", "'-'", "']'",
  "'='", "'/'", "'#'", "'%'", "$accept", "stylesheet", "webkit_rule",
  "webkit_keyframe_rule", "webkit_decls", "webkit_value",
  "webkit_mediaquery", "webkit_selector", "maybe_space", "maybe_sgml",
  "maybe_charset", "closing_brace", "charset", "ignored_charset",
  "rule_list", "valid_rule", "rule", "block_rule_list", "block_valid_rule",
  "block_rule", "at_import_header_end_maybe_space", "before_import_rule",
  "import", "namespace", "maybe_ns_prefix", "string_or_uri",
  "media_feature", "maybe_media_value", "media_query_exp",
  "media_query_exp_list", "maybe_and_media_query_exp_list",
  "maybe_media_restrictor", "media_query", "maybe_media_list",
  "media_list", "at_rule_body_start", "before_media_rule",
  "at_rule_header_end_maybe_space", "media", "medium",
  "before_keyframes_rule", "keyframes", "keyframe_name", "keyframes_rule",
  "keyframe_rule", "key_list", "key", "before_page_rule", "page",
  "page_selector", "declarations_and_margins", "margin_box", "$@1",
  "margin_sym", "before_font_face_rule", "font_face", "region_selector",
  "before_region_rule", "region", "combinator", "maybe_unary_operator",
  "unary_operator", "maybe_space_before_declaration",
  "before_selector_list", "at_rule_header_end", "ruleset", "selector_list",
  "selector_with_trailing_whitespace", "selector", "namespace_selector",
  "simple_selector", "simple_selector_list", "element_name",
  "specifier_list", "specifier", "class", "attr_name", "attrib", "match",
  "ident_or_string", "pseudo_page", "pseudo", "declaration_list",
  "decl_list", "declaration", "property", "prio", "expr", "operator",
  "term", "unary_term", "function", "calc_func_term", "calc_func_operator",
  "calc_func_paren_expr", "calc_func_expr", "calc_func_expr_list",
  "calc_function", "min_or_max", "min_or_max_function", "save_block",
  "invalid_at", "invalid_rule", "invalid_block", "invalid_block_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,    58,    46,    91,
      42,   124,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   123,   125,    59,
      40,    41,    44,    43,   126,    62,    45,    93,    61,    47,
      35,    37
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   112,   113,   113,   113,   113,   113,   113,   113,   114,
     115,   116,   117,   118,   119,   120,   120,   121,   121,   121,
     122,   122,   123,   123,   124,   124,   124,   125,   125,   126,
     126,   127,   127,   127,   127,   127,   127,   127,   127,   128,
     128,   128,   128,   129,   129,   130,   130,   130,   130,   131,
     131,   131,   131,   131,   131,   132,   133,   134,   134,   134,
     134,   134,   135,   135,   135,   135,   136,   136,   137,   137,
     138,   139,   139,   140,   141,   141,   142,   142,   143,   143,
     143,   144,   144,   145,   145,   146,   146,   146,   147,   148,
     149,   150,   150,   150,   151,   152,   153,   154,   154,   155,
     155,   156,   157,   157,   158,   158,   159,   160,   160,   160,
     161,   161,   161,   161,   162,   162,   164,   163,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   166,   167,   167,   167,   168,   169,
     170,   171,   171,   171,   172,   172,   173,   173,   174,   175,
     176,   177,   178,   178,   178,   179,   180,   180,   180,   180,
     180,   181,   181,   181,   182,   182,   182,   182,   182,   182,
     183,   183,   183,   184,   184,   185,   185,   185,   186,   186,
     186,   186,   186,   187,   188,   189,   189,   189,   189,   190,
     190,   190,   190,   190,   190,   191,   191,   192,   193,   193,
     193,   193,   193,   193,   193,   194,   194,   194,   194,   194,
     194,   194,   195,   195,   195,   195,   195,   195,   195,   195,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   197,
     198,   198,   199,   199,   199,   199,   199,   200,   200,   200,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   203,   203,   203,   203,   204,   204,   204,   205,
     205,   205,   205,   206,   207,   207,   207,   207,   207,   208,
     208,   209,   209,   210,   210,   211,   211,   212,   212,   213,
     213,   214,   215,   215,   216,   216
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     2,     2,     2,     2,     2,     2,     6,
       6,     5,     5,     5,     5,     0,     2,     0,     2,     2,
       0,     1,     1,     1,     5,     3,     3,     5,     3,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     7,     7,     7,
       4,     4,     6,     6,     3,     3,     0,     2,     1,     1,
       2,     0,     4,     9,     1,     5,     0,     3,     0,     1,
       1,     1,     4,     0,     1,     1,     4,     2,     0,     0,
       1,    10,     8,     4,     2,     0,    10,     1,     1,     0,
       3,     6,     1,     5,     2,     1,     0,    10,     4,     4,
       1,     2,     1,     0,     1,     4,     0,     7,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     8,     4,     4,     1,     0,
      10,     2,     2,     2,     1,     0,     1,     1,     1,     0,
       0,     8,     1,     4,     2,     2,     1,     1,     2,     3,
       2,     1,     2,     2,     1,     2,     1,     2,     3,     2,
       1,     5,     2,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     2,     2,     4,     8,     5,     9,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       6,     6,     7,     6,     6,     1,     2,     1,     3,     1,
       2,     2,     3,     3,     4,     3,     5,     4,     4,     6,
       5,     5,     2,     6,     6,     2,     3,     4,     2,     2,
       2,     0,     1,     3,     2,     3,     2,     2,     2,     0,
       1,     2,     2,     2,     2,     3,     2,     2,     2,     2,
       5,     1,     1,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     5,     4,     4,     3,     1,     5,     2,     2,
       2,     2,     2,     6,     2,     3,     3,     1,     2,     1,
       4,     5,     3,     1,     1,     5,     3,     1,     2,     3,
       3,     2,     5,     3,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
      15,     0,     0,     0,     0,     0,     0,     0,    15,    15,
      15,    15,    15,    15,    20,    15,    15,    15,    15,    15,
      15,     1,     3,     8,     4,     5,     6,     7,    16,     0,
      17,    21,   149,   148,     0,   145,     0,    78,     0,     0,
       0,    29,     0,    15,     0,    37,    36,     0,    32,     0,
      35,     0,    33,     0,    34,     0,    38,     0,    31,   209,
      15,    15,     0,     0,     0,   205,     0,   105,   147,   146,
      15,    15,   102,     0,   144,    15,    15,    15,    15,    15,
      15,    15,    15,    15,    15,    15,    15,    15,    15,    15,
      15,    15,    15,    15,    15,    15,    15,    15,    15,    15,
      15,    15,    15,    15,    15,    15,    15,    15,    15,    15,
     303,   304,    15,    15,    15,     0,     0,   232,   240,   251,
     252,    15,   253,    79,    80,    74,    81,    15,     0,   173,
     179,   178,     0,     0,    15,   174,   161,     0,   157,     0,
       0,   156,   164,     0,   175,   180,   181,   182,     0,    26,
      25,    15,    19,    18,     0,     0,    66,     0,     0,    15,
      15,     0,     0,     0,     0,    15,   314,     0,   229,   225,
      15,    11,   210,   206,     0,    15,     0,   222,    15,   228,
       0,     0,   104,   242,   243,   248,     0,   275,   273,   272,
     274,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   244,   257,   256,   255,   276,
     277,   278,   279,   280,   281,   246,     0,     0,   247,   249,
     254,    15,   241,   236,    12,    15,    15,     0,     0,     0,
       0,     0,    13,   163,   198,     0,    15,    15,    15,   183,
       0,   162,   154,    14,    15,   158,   160,   155,    15,    15,
      15,     0,   173,   174,   167,     0,     0,   177,   176,     0,
       0,     0,    15,     0,    40,    39,    17,    42,    41,    65,
      64,    15,     0,     9,     0,    55,     0,    78,     0,     0,
       0,   113,     0,    90,     0,     0,     0,   215,   208,     0,
      15,     0,    15,     0,   212,    15,   213,     0,    10,    15,
      15,     0,   285,    15,     0,   302,    15,    15,     0,   286,
      15,   297,     0,   245,   238,   237,   233,   235,   306,     0,
       0,    15,    15,    15,    76,   199,   145,     0,     0,    15,
       0,     0,     0,     0,   141,   142,   143,   159,     0,    23,
      22,   313,     0,    24,   311,     0,     0,    30,    67,    68,
      69,    15,    60,    61,    15,    85,     0,    88,    93,    98,
      97,    15,   109,   108,   110,     0,    15,   112,   137,   136,
      88,   150,     0,    88,    15,   315,     0,   218,     0,   217,
     214,   227,     0,     0,   145,    15,   284,   283,    15,     0,
       0,   288,   294,   298,    15,    15,     0,     0,    15,     0,
      15,    15,    78,    94,     0,    15,    82,    15,    15,     0,
     170,     0,    15,   184,    15,     0,   190,   191,   192,   193,
     194,   185,   189,    15,     0,     0,    15,    28,   310,   309,
       0,    83,    87,    15,     0,    15,     0,   111,   197,     0,
      15,     0,    15,   216,    15,   220,    15,     0,     0,     0,
     103,   250,   282,     0,     0,   291,   301,   289,   290,   292,
     295,   296,   305,     0,    75,    15,    15,    15,    78,     0,
       0,    15,   172,     0,     0,   187,    15,     0,   312,     0,
      62,    63,     0,     0,    78,    88,    43,    88,    88,     0,
      88,     0,   230,   219,   223,   224,   101,    15,     0,     0,
       0,    70,    71,    77,   203,   201,     0,   200,    15,   204,
       0,   196,   195,    15,    27,    58,    57,    59,    86,    15,
       0,    15,    15,     0,    15,     0,   287,    15,    15,     0,
     202,     0,    15,     0,    43,     0,   307,    49,    17,    53,
      52,    54,    48,    46,    47,    45,    92,    51,    50,    99,
       0,   135,    43,   151,   293,     0,    15,   171,     0,   186,
       0,   308,    44,   145,     0,   114,     0,     0,    73,   188,
      91,    96,    15,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   107,
      15,   116,   140,    72,   100,     0,    15,   115,     0,    15,
       0,     0,   117
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     7,     8,     9,    10,    11,    12,    13,    33,    41,
      30,   536,    31,   264,   154,    43,   266,   520,   537,   538,
     276,    44,   539,   540,   272,   351,   467,   529,   125,   126,
     406,   127,   355,   482,   356,   435,    47,   278,   541,   324,
      49,   542,   361,   563,    70,    71,    72,    51,   543,   366,
     564,   590,   596,   591,    53,   544,   371,    55,    56,   251,
      73,   115,    34,    57,   286,   545,   137,   138,   139,   140,
     141,   411,   142,   143,   144,   145,   332,   146,   423,   513,
     367,   147,    63,    64,    65,    66,   445,   116,   227,   117,
     118,   119,   310,   399,   311,   312,   320,   120,   121,   122,
     546,   547,   548,   166,   228
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -515
static const yytype_int16 yypact[] =
{
     609,   -52,   -41,   -24,    82,   191,   104,   243,  -515,  -515,
    -515,  -515,  -515,  -515,    42,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,   200,   200,   200,   200,   200,   200,  -515,   381,
    -515,  -515,   530,   200,   335,   205,  1303,   259,   566,   160,
      50,   435,   408,  -515,   224,  -515,  -515,   206,  -515,   234,
    -515,   279,  -515,   245,  -515,   273,  -515,   585,  -515,   339,
    -515,  -515,   313,   246,  1747,   382,    57,  -515,  -515,  -515,
    -515,  -515,  -515,   285,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,  1188,  1118,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,   124,  -515,   291,   398,
    -515,  -515,   415,   389,  -515,   404,  -515,   122,   585,   179,
     599,  -515,   499,   472,  -515,  -515,  -515,  -515,   428,  -515,
    -515,  -515,  -515,  -515,  1899,   385,   360,   247,   405,  -515,
    -515,   386,   208,   450,    71,  -515,  -515,   460,   200,   200,
    -515,  -515,   395,   363,  1767,  -515,  1667,  -515,  -515,  -515,
     249,   199,  -515,   200,   200,   200,   391,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,  1218,  1393,   200,   200,
     200,  -515,  -515,  -515,  -515,  -515,  -515,  1568,   608,  1442,
     299,   242,  -515,  -515,  -515,   462,  -515,  -515,  -515,  -515,
     465,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,   585,  -515,  -515,   499,   528,   934,  -515,  -515,   181,
     115,   429,  -515,   529,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,   103,  -515,   401,   200,   103,   319,   413,   187,
     423,   494,   426,   200,   451,   585,   452,   200,   439,  1303,
    -515,   533,  -515,   429,   200,  -515,   200,   812,  -515,  -515,
    -515,   394,  -515,  -515,   912,  -515,  -515,  -515,  1273,  -515,
    -515,  -515,   240,   200,   200,   200,  -515,   429,  -515,   166,
     400,  -515,  -515,  -515,   492,  -515,    36,   566,   566,   398,
     404,   538,    56,   566,   200,   200,   200,  -515,   943,  -515,
    -515,  -515,   557,  -515,  -515,   251,   440,   435,   200,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,   143,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,   542,   547,  -515,  -515,  -515,  -515,
    -515,  -515,   156,  -515,  -515,  -515,   712,   200,   443,   200,
     200,  1568,   712,   379,   205,  -515,   200,  -515,  -515,   434,
    1492,  -515,   200,  -515,  -515,  -515,   556,   558,  -515,  1807,
    -515,  -515,   259,   200,   459,  -515,  -515,  -515,  -515,   483,
    -515,    31,  -515,   200,  -515,   361,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,   355,   181,  -515,  -515,  -515,  -515,
     256,   322,  -515,  -515,   468,  -515,   471,  -515,  -515,   481,
    -515,   495,  -515,   200,  -515,  -515,  -515,   712,  1685,   482,
    -515,   200,   200,   488,   211,   200,   200,  -515,  -515,   200,
    -515,  -515,   200,  1492,  -515,  -515,  -515,  -515,   259,    39,
     126,  -515,  -515,   237,   172,  -515,  -515,   437,  -515,   260,
    -515,  -515,   118,   189,   259,  -515,   200,  -515,  -515,   335,
    -515,   335,   200,   200,  -515,  -515,  -515,  -515,   218,   231,
     193,   200,   410,   124,  -515,  -515,   230,  -515,  -515,  -515,
     437,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,
    1142,  -515,  -515,   110,  -515,   110,   200,  -515,  -515,   490,
    -515,   566,  -515,    34,   200,   181,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,   200,
     335,  -515,   200,  -515,   200,  1303,  -515,  -515,    43,  -515,
    1142,  -515,   435,    40,  1012,  -515,  1142,  1015,   200,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,   200,   200,   379,  -515,  -515,   183,  -515,
     379,   110,  -515
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,     0,  -264,
    -515,  -238,  -515,  -515,  -515,   441,  -515,  -483,  -515,  -515,
    -515,  -515,    -9,    -7,  -515,   318,  -515,  -515,   195,   139,
    -515,  -343,   -36,  -515,   180,  -316,  -515,  -155,    -6,  -515,
    -515,    -2,  -515,  -515,    47,  -515,   238,  -515,    -1,  -515,
    -515,  -515,  -515,  -515,  -515,     1,  -515,  -515,  -515,  -515,
     298,   -32,   -59,  -515,   -43,     2,   -51,  -515,   294,   388,
    -134,  -515,   497,  -105,  -119,  -515,   303,  -515,   220,   131,
     300,  -515,  -367,  -515,   598,  -515,  -252,  -178,  -515,   438,
    -101,  -515,   267,  -515,   287,  -224,  -515,  -515,  -515,  -515,
    -514,   545,   549,   163,   -37
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -301
static const yytype_int16 yytable[] =
{
      14,   128,   347,    74,   245,   319,   164,   284,    22,    23,
      24,    25,    26,    27,   222,    32,   449,    35,    36,    37,
      38,   341,   167,    45,   258,    46,    48,   174,   176,    40,
      50,    52,   472,    54,    58,   255,   -15,   256,   304,    28,
     339,    28,   156,   157,    28,    15,   570,    28,    28,   407,
     408,   560,   592,    67,   440,    28,    16,   442,   177,   465,
     168,   169,   151,   416,   417,   418,   419,   420,    29,   566,
     180,   181,   242,    17,   178,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     339,   376,   218,   219,   220,   349,   309,   337,   515,   382,
      28,   229,   523,   242,   525,   465,   230,   231,   309,   -15,
     448,    28,   -15,   -15,   240,   291,   258,   258,   340,    68,
     504,   559,    69,    68,   432,    45,    69,    46,    48,   338,
     569,   260,    50,    52,   148,    54,    58,   242,   275,   277,
     279,   281,   283,   421,   422,   287,   454,   393,  -150,   519,
     289,   521,   522,   244,   524,   294,   296,    28,   297,    18,
     246,   339,   -15,   565,   247,   308,   394,   478,    28,   -84,
     432,   350,    28,   410,   412,   494,    19,   308,    28,   359,
     360,    20,   150,   447,    28,    28,   436,   391,   340,   282,
      28,   439,   393,   -15,   343,   148,   -15,   516,    67,   258,
     243,   313,   342,    28,   244,   314,   315,   505,   597,   179,
     159,   394,   393,   601,   372,    28,   326,   327,   328,   499,
    -150,   393,    28,    21,   333,   433,   158,    28,   334,   335,
     336,   394,    28,  -138,    28,   322,    28,   148,   244,   149,
     394,    28,   345,   426,    28,    28,   160,  -299,  -299,   396,
     162,   348,   397,   509,   562,   398,  -152,  -152,   148,   340,
     599,  -152,   248,   249,   250,   551,   -84,   553,   -84,   309,
     377,   433,   379,   323,    74,   380,   299,   561,   309,   383,
     384,   300,   161,   386,    28,   -15,   389,   390,    68,   163,
     392,    69,   -15,   434,   396,   123,   124,   397,   270,   527,
     398,   402,   403,   404,    28,   571,   589,    28,   441,   413,
     170,   530,  -300,  -300,   396,   -78,    59,   397,   507,   508,
     398,   395,   323,   396,   171,   273,   397,   298,    60,   398,
     427,   430,    74,   148,   431,   480,   246,   321,   308,   514,
     247,   283,   309,   602,   182,    28,   283,   308,   416,   417,
     418,   419,   420,   271,   443,   123,   124,   567,   123,   124,
      59,   489,    39,   491,    28,   451,   -15,   280,   452,   232,
      61,   -15,    60,   -15,   455,   456,    28,   557,   459,   -15,
     462,   463,   239,   -15,   301,   468,   274,   469,   470,   155,
     -15,   473,   474,   -15,   413,    28,   -90,   -15,   -90,   233,
     -15,   -15,   -78,   477,   344,   241,   479,   528,   234,   259,
      62,   308,   235,   484,    61,   486,   148,   353,   165,    28,
     152,   153,    28,   363,   492,   369,   493,   453,   518,   511,
     512,   375,  -153,  -153,   498,   285,   375,  -153,   248,   249,
     250,   288,   292,   550,    28,   500,   501,   502,   475,   422,
      28,   506,   466,   257,    62,   325,   510,  -166,   329,   148,
     375,   175,   148,   -15,   269,   330,   136,   130,   131,   132,
     133,   134,   148,   -15,   290,   385,   -15,   526,   148,    28,
     352,   400,   401,   230,   236,   237,   238,   364,   531,   429,
     357,   365,   358,   533,   130,   131,   132,   133,   134,   534,
     148,   549,   362,   148,   552,   368,   148,   554,   555,   257,
     346,    74,   558,  -169,   378,    28,   148,   148,   374,   428,
     148,   375,   446,   130,   131,   132,   133,   134,   370,   373,
     405,   414,   -56,  -106,   -89,  -134,   568,    42,   425,   365,
     438,   457,   -95,   458,   471,   485,  -139,   593,   487,  -166,
    -166,    28,   594,  -166,  -166,  -166,  -166,  -166,   488,   129,
     496,   130,   131,   132,   133,   134,   135,   136,   375,   497,
     595,   556,   490,   481,   354,   265,   598,   464,   129,   600,
     130,   131,   132,   133,   134,   135,   136,   503,  -234,   317,
     572,   483,   252,  -234,   130,   131,   132,   133,   134,   253,
    -234,  -234,   450,  -234,   409,  -169,  -169,   424,   331,  -169,
    -169,  -169,  -169,  -169,   415,   476,  -234,   254,     1,     2,
       3,   532,     4,     5,     6,   517,  -234,  -234,  -234,  -234,
    -234,  -234,  -234,  -234,  -234,  -234,  -234,  -234,  -234,  -234,
    -234,  -234,   173,  -234,   437,   316,   460,  -234,  -234,  -234,
    -234,  -234,  -234,  -234,  -234,  -234,  -234,  -234,  -234,  -234,
    -234,  -234,  -234,  -234,  -234,  -234,   461,  -234,  -234,  -234,
    -234,  -234,  -234,  -234,  -234,  -234,  -234,  -234,   344,   267,
    -234,  -234,  -234,   268,  -234,  -234,  -234,  -234,     0,  -234,
    -234,  -234,  -231,   223,  -234,     0,     0,  -234,  -234,  -234,
       0,     0,     0,     0,  -239,  -239,     0,  -239,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -239,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -231,  -231,  -231,  -231,  -231,  -231,  -231,  -231,  -231,  -231,
    -231,  -231,  -231,  -231,  -231,  -231,     0,   444,     0,     0,
       0,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,
    -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,
       0,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,
    -239,  -239,     0,     0,  -239,  -239,  -239,     0,  -239,   148,
    -231,  -231,  -226,   381,   225,  -239,     0,    28,  -239,     0,
       0,   226,  -239,  -239,    75,    76,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -226,  -226,  -226,  -226,  -226,  -226,  -226,  -226,  -226,  -226,
    -226,  -226,  -226,  -226,  -226,  -226,     0,     0,     0,     0,
       0,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
       0,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,     0,     0,   109,   110,   111,     0,   112,  -226,
    -226,  -226,   387,   223,     0,    68,     0,     0,    69,     0,
       0,     0,   113,   114,  -239,  -239,     0,  -239,     0,     0,
       0,     0,     0,     0,     0,   257,     0,     0,     0,  -165,
    -239,     0,     0,     0,   257,     0,     0,     0,  -168,   130,
     131,   132,   133,   134,     0,     0,     0,     0,   130,   131,
     132,   133,   134,     0,     0,     0,     0,     0,     0,     0,
       0,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,
    -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,
       0,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,
    -239,  -239,     0,     0,  -239,  -239,  -239,     0,  -239,   148,
       0,     0,   339,   388,   225,  -239,   223,     0,  -239,     0,
     -15,   226,  -239,  -239,     0,     0,     0,  -239,  -239,     0,
    -239,  -165,  -165,     0,     0,  -165,  -165,  -165,  -165,  -165,
    -168,  -168,     0,  -239,  -168,  -168,  -168,  -168,  -168,     0,
     573,   574,   575,   576,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,     0,     0,     0,     0,
       0,     0,     0,     0,  -239,  -239,  -239,  -239,  -239,  -239,
    -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,
    -239,  -239,  -239,     0,  -239,  -239,  -239,  -239,  -239,  -239,
    -239,  -239,  -239,  -239,  -239,     0,     0,  -239,  -239,  -239,
     340,  -239,   148,     0,     0,     0,   -15,   225,  -239,   223,
       0,  -239,     0,     0,   226,  -239,  -239,     0,     0,     0,
    -239,  -239,     0,  -239,     0,     0,     0,     0,     0,     0,
       0,     0,   339,   535,     0,     0,  -239,     0,     0,     0,
       0,     0,     0,     0,     0,  -149,     0,  -149,  -149,  -149,
    -149,  -149,  -149,  -149,   -56,  -106,   -89,  -134,     0,    42,
       0,     0,     0,     0,   -95,     0,     0,  -239,  -239,  -239,
    -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,
    -239,  -239,  -239,  -239,  -239,  -239,   263,  -239,  -239,  -239,
    -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,     0,     0,
    -239,  -239,  -239,     0,  -239,   148,   224,     0,     0,   302,
     225,  -239,     0,    28,  -239,     0,     0,   226,  -239,  -239,
      75,    76,     0,    77,     0,     0,     0,     0,     0,     0,
     340,     0,     0,     0,     0,     0,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,   221,     0,    98,    99,   100,
     101,   102,   103,   104,   105,   106,     0,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,    28,     0,
     109,   110,   111,     0,   112,    75,    76,     0,    77,   303,
       0,    68,     0,     0,    69,     0,     0,     0,   113,   114,
       0,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
       0,     0,    98,    99,   100,   101,   102,   103,   104,   105,
     106,     0,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,     0,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,     0,   305,   109,   110,   111,    28,   112,
       0,     0,     0,     0,     0,     0,    68,     0,     0,    69,
       0,     0,     0,   113,   114,     0,     0,     0,     0,     0,
       0,   306,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   318,     0,     0,     0,    28,     0,     0,
       0,     0,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
     306,     0,    98,    99,   100,   101,   102,   103,   104,   105,
     106,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   307,     0,     0,    68,    28,     0,    69,
       0,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,     0,
     306,    98,    99,   100,   101,   102,   103,   104,   105,   106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   307,     0,     0,    68,     0,     0,    69,     0,
       0,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,     0,
       0,    98,    99,   100,   101,   102,   103,   104,   105,   106,
      75,    76,     0,    77,     0,     0,     0,     0,     0,     0,
       0,     0,   307,     0,     0,    68,    78,     0,    69,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,     0,     0,
     109,   110,   111,     0,   112,     0,     0,   -15,   293,     0,
       0,    68,   -15,     0,    69,     0,     0,     0,   113,   114,
     -15,     0,     0,     0,     0,  -221,   495,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   -15,   -15,   -15,   -15,   -15,
     -15,   -15,   -15,   -15,   -15,   -15,   -15,   -15,   -15,   -15,
     -15,     0,   -15,  -221,  -221,  -221,  -221,  -221,  -221,  -221,
    -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,     0,
       0,     0,     0,     0,     0,     0,     0,  -207,   172,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      60,     0,   -15,     0,   -15,   -15,   295,  -211,   293,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -221,  -221,  -221,  -207,  -207,  -207,  -207,  -207,
    -207,  -207,  -207,  -207,  -207,  -207,  -207,  -207,  -207,  -207,
    -207,     0,    61,     0,     0,  -211,  -211,  -211,  -211,  -211,
    -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,
    -211,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   306,     0,     0,     0,     0,
       0,     0,    62,     0,   148,  -207,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -211,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,     0,     0,    98,    99,   100,   101,
     102,   103,   104,   105,   106,     0,     0,     0,     0,    -2,
     261,     0,     0,     0,     0,     0,     0,   307,     0,     0,
      68,     0,  -149,    69,  -149,  -149,  -149,  -149,  -149,  -149,
    -149,   -56,  -106,   -89,  -134,   262,    42,     0,     0,     0,
       0,   -95,     0,     0,     0,  -139,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   263
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-515))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       0,    37,   266,    35,   138,   229,    57,   162,     8,     9,
      10,    11,    12,    13,   115,    15,   383,    17,    18,    19,
      20,   259,    59,    32,   143,    32,    32,    64,    65,    29,
      32,    32,     1,    32,    32,   140,     5,   142,   216,     5,
       0,     5,    42,    43,     5,    97,   560,     5,     5,    13,
      14,   534,   566,    13,   370,     5,    97,   373,     1,   402,
      60,    61,    12,     7,     8,     9,    10,    11,    26,   552,
      70,    71,     1,    97,    17,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
       0,   289,   112,   113,   114,    12,   217,   251,     0,   297,
       5,   121,   489,     1,   491,   468,   126,   127,   229,     5,
     382,     5,   101,   102,   134,   172,   255,   256,    98,   103,
     101,   107,   106,   103,     1,   154,   106,   154,   154,   254,
     107,   151,   154,   154,    97,   154,   154,     1,   158,   159,
     160,   161,   162,   107,   108,   165,   390,     1,    97,   485,
     170,   487,   488,   102,   490,   175,   176,     5,   178,    97,
       1,     0,    58,   550,     5,   217,    20,   425,     5,     0,
       1,    88,     5,   327,   328,   447,     5,   229,     5,    12,
      13,    97,    39,   381,     5,     5,   361,   308,    98,     1,
       5,   366,     1,     5,    99,    97,     5,    99,    13,   338,
      98,   221,   259,     5,   102,   225,   226,   101,   595,    66,
      24,    20,     1,   600,   285,     5,   236,   237,   238,   463,
      97,     1,     5,     0,   244,   102,    22,     5,   248,   249,
     250,    20,     5,    97,     5,    13,     5,    97,   102,    99,
      20,     5,   262,    12,     5,     5,    32,   101,   102,   103,
      25,   271,   106,   101,   538,   109,    97,    98,    97,    98,
      97,   102,   103,   104,   105,   523,    97,   525,    99,   390,
     290,   102,   292,   100,   326,   295,    97,   535,   399,   299,
     300,   102,    23,   303,     5,    97,   306,   307,   103,    36,
     310,   106,   101,   356,   103,    56,    57,   106,   155,   101,
     109,   321,   322,   323,     5,   563,   564,     5,   371,   329,
      17,   101,   101,   102,   103,    13,     1,   106,   101,   102,
     109,   101,   100,   103,    98,    98,   106,    98,    13,   109,
      99,   351,   384,    97,   354,    99,     1,    58,   390,    99,
       5,   361,   463,   601,    79,     5,   366,   399,     7,     8,
       9,    10,    11,    13,   374,    56,    57,   555,    56,    57,
       1,   440,     1,   442,     5,   385,     5,     1,   388,    98,
      55,     5,    13,    12,   394,   395,     5,   531,   398,    13,
     400,   401,    13,    17,    13,   405,     1,   407,   408,     1,
       5,   411,   412,     5,   414,     5,    97,    12,    99,    21,
      12,    13,   100,   423,   261,    21,   426,    17,    13,     1,
      95,   463,    17,   433,    55,   435,    97,   274,    99,     5,
       5,     6,     5,   280,   444,   282,   446,    13,   484,    12,
      13,   288,    97,    98,   454,     5,   293,   102,   103,   104,
     105,     1,    99,   522,     5,   465,   466,   467,   107,   108,
       5,   471,    13,     1,    95,    13,   476,     5,    13,    97,
     317,    99,    97,    97,    99,    20,    21,    15,    16,    17,
      18,    19,    97,    88,    99,   101,    88,   497,    97,     5,
      99,   101,   102,   503,    89,    90,    91,    13,   508,   346,
      97,    17,    99,   513,    15,    16,    17,    18,    19,   519,
      97,   521,    99,    97,   524,    99,    97,   527,   528,     1,
       1,   563,   532,     5,     1,     5,    97,    97,    99,    99,
      97,   378,    99,    15,    16,    17,    18,    19,    97,    97,
      58,    13,    22,    23,    24,    25,   556,    27,     1,    17,
      13,     5,    32,     5,    81,    97,    36,   567,    97,    97,
      98,     5,   572,   101,   102,   103,   104,   105,    97,    13,
      98,    15,    16,    17,    18,    19,    20,    21,   425,   101,
     590,   101,    97,   430,   276,   154,   596,   402,    13,   599,
      15,    16,    17,    18,    19,    20,    21,   468,     0,     1,
     563,   431,    13,     5,    15,    16,    17,    18,    19,    20,
      12,    13,   384,    15,   326,    97,    98,   333,   240,   101,
     102,   103,   104,   105,   331,   415,    28,   140,    29,    30,
      31,   510,    33,    34,    35,   482,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    64,    55,   364,   227,   399,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,   399,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,   535,   154,
      92,    93,    94,   154,    96,    97,    98,    99,    -1,   101,
     102,   103,     0,     1,   106,    -1,    -1,   109,   110,   111,
      -1,    -1,    -1,    -1,    12,    13,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    55,    -1,    -1,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      98,    99,     0,     1,   102,   103,    -1,     5,   106,    -1,
      -1,   109,   110,   111,    12,    13,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      98,    99,     0,     1,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,   111,    12,    13,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,     5,
      28,    -1,    -1,    -1,     1,    -1,    -1,    -1,     5,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      -1,    -1,     0,   101,   102,   103,     1,    -1,   106,    -1,
       5,   109,   110,   111,    -1,    -1,    -1,    12,    13,    -1,
      15,    97,    98,    -1,    -1,   101,   102,   103,   104,   105,
      97,    98,    -1,    28,   101,   102,   103,   104,   105,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    -1,    92,    93,    94,
      98,    96,    97,    -1,    -1,    -1,   101,   102,   103,     1,
      -1,   106,    -1,    -1,   109,   110,   111,    -1,    -1,    -1,
      12,    13,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     0,     1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    -1,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    -1,    27,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    54,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    -1,
      92,    93,    94,    -1,    96,    97,    98,    -1,    -1,     1,
     102,   103,    -1,     5,   106,    -1,    -1,   109,   110,   111,
      12,    13,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    28,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,     5,    -1,
      92,    93,    94,    -1,    96,    12,    13,    -1,    15,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,   111,
      -1,    28,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      -1,    -1,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,     1,    92,    93,    94,     5,    96,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,    -1,    -1,     5,    -1,    -1,
      -1,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      28,    -1,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,    -1,    -1,   103,     5,    -1,   106,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    -1,
      28,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    -1,
      -1,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      12,    13,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,    -1,    -1,   103,    28,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    -1,
      92,    93,    94,    -1,    96,    -1,    -1,     0,     1,    -1,
      -1,   103,     5,    -1,   106,    -1,    -1,    -1,   110,   111,
      13,    -1,    -1,    -1,    -1,     0,     1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    55,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,     1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    95,    -1,    97,    98,    99,     0,     1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    55,    -1,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    97,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    -1,    -1,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    -1,    -1,    -1,     0,
       1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,    -1,
     103,    -1,    13,   106,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    29,    30,    31,    33,    34,    35,   113,   114,   115,
     116,   117,   118,   119,   120,    97,    97,    97,    97,     5,
      97,     0,   120,   120,   120,   120,   120,   120,     5,    26,
     122,   124,   120,   120,   174,   120,   120,   120,   120,     1,
     120,   121,    27,   127,   133,   134,   135,   148,   150,   152,
     153,   159,   160,   166,   167,   169,   170,   175,   177,     1,
      13,    55,    95,   194,   195,   196,   197,    13,   103,   106,
     156,   157,   158,   172,   173,    12,    13,    15,    28,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    92,
      93,    94,    96,   110,   111,   173,   199,   201,   202,   203,
     209,   210,   211,    56,    57,   140,   141,   143,   144,    13,
      15,    16,    17,    18,    19,    20,    21,   178,   179,   180,
     181,   182,   184,   185,   186,   187,   189,   193,    97,    99,
     215,    12,     5,     6,   126,     1,   120,   120,    22,    24,
      32,    23,    25,    36,   178,    99,   215,   216,   120,   120,
      17,    98,     1,   196,   216,    99,   216,     1,    17,   215,
     120,   120,    79,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,    77,   202,     1,    98,   102,   109,   200,   216,   120,
     120,   120,    98,    21,    13,    17,    89,    90,    91,    13,
     120,    21,     1,    98,   102,   182,     1,     5,   103,   104,
     105,   171,    13,    20,   184,   185,   185,     1,   186,     1,
     120,     1,    26,    54,   125,   127,   128,   213,   214,    99,
     215,    13,   136,    98,     1,   120,   132,   120,   149,   120,
       1,   120,     1,   120,   149,     5,   176,   120,     1,   120,
      99,   216,    99,     1,   120,    99,   120,   120,    98,    97,
     102,    13,     1,   101,   199,     1,    28,   100,   173,   202,
     204,   206,   207,   120,   120,   120,   201,     1,     1,   207,
     208,    58,    13,   100,   151,    13,   120,   120,   120,    13,
      20,   181,   188,   120,   120,   120,   120,   182,   185,     0,
      98,   123,   216,    99,   215,   120,     1,   121,   120,    12,
      88,   137,    99,   215,   137,   144,   146,    97,    99,    12,
      13,   154,    99,   215,    13,    17,   161,   192,    99,   215,
      97,   168,   178,    97,    99,   215,   199,   120,     1,   120,
     120,     1,   199,   120,   120,   101,   120,     0,   101,   120,
     120,   202,   120,     1,    20,   101,   103,   106,   109,   205,
     101,   102,   120,   120,   120,    58,   142,    13,    14,   172,
     182,   183,   182,   120,    13,   188,     7,     8,     9,    10,
      11,   107,   108,   190,   180,     1,    12,    99,    99,   215,
     120,   120,     1,   102,   176,   147,   149,   192,    13,   149,
     147,   176,   147,   120,    55,   198,    99,   199,   198,   194,
     158,   120,   120,    13,   207,   120,   120,     5,     5,   120,
     204,   206,   120,   120,   140,   143,    13,   138,   120,   120,
     120,    81,     1,   120,   120,   107,   190,   120,   123,   120,
      99,   215,   145,   146,   120,    97,   120,    97,    97,   174,
      97,   174,   120,   120,   198,     1,    98,   101,   120,   207,
     120,   120,   120,   141,   101,   101,   120,   101,   102,   101,
     120,    12,    13,   191,    99,     0,    99,   215,   144,   147,
     129,   147,   147,   194,   147,   194,   120,   101,    17,   139,
     101,   120,   191,   120,   120,     1,   123,   130,   131,   134,
     135,   150,   153,   160,   167,   177,   212,   213,   214,   120,
     174,   123,   120,   123,   120,   120,   101,   182,   120,   107,
     129,   123,   121,   155,   162,   194,   129,   199,   120,   107,
     212,   123,   156,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,   123,
     163,   165,   212,   120,   120,   120,   164,   194,   120,    97,
     120,   194,   123
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (parser, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, parser)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, parser); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, CSSParser* parser)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, parser)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    CSSParser* parser;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (parser);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, CSSParser* parser)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, parser)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    CSSParser* parser;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, parser);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, CSSParser* parser)
#else
static void
yy_reduce_print (yyvsp, yyrule, parser)
    YYSTYPE *yyvsp;
    int yyrule;
    CSSParser* parser;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , parser);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, parser); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, CSSParser* parser)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, parser)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    CSSParser* parser;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (parser);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (CSSParser* parser);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (CSSParser* parser)
#else
int
yyparse (parser)
    CSSParser* parser;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 9:

/* Line 1806 of yacc.c  */
#line 391 "generated/CSSGrammar.y"
    {
parser->m_rule = (yyvsp[(4) - (6)].rule);
}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 397 "generated/CSSGrammar.y"
    {
parser->m_keyframe = (yyvsp[(4) - (6)].keyframe);
}
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 403 "generated/CSSGrammar.y"
    { 

}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 409 "generated/CSSGrammar.y"
    {
if ((yyvsp[(4) - (5)].valueList)) {
parser->m_valueList = parser->sinkFloatingValueList((yyvsp[(4) - (5)].valueList));
int oldParsedProperties = parser->m_parsedProperties.size();
if (!parser->parseValue(parser->m_id, parser->m_important))
parser->rollbackLastProperties(parser->m_parsedProperties.size() - oldParsedProperties);
parser->m_valueList = nullptr;
}
}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 421 "generated/CSSGrammar.y"
    {
parser->m_mediaQuery = parser->sinkFloatingMediaQuery((yyvsp[(4) - (5)].mediaQuery));
}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 427 "generated/CSSGrammar.y"
    {
if ((yyvsp[(4) - (5)].selectorList)) {
if (parser->m_selectorListForParseSelector)
parser->m_selectorListForParseSelector->adoptSelectorVector(*(yyvsp[(4) - (5)].selectorList));
}
}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 448 "generated/CSSGrammar.y"
    {
}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 458 "generated/CSSGrammar.y"
    {
if (parser->m_styleSheet)
parser->m_styleSheet->parserSetEncodingFromCharsetRule((yyvsp[(3) - (5)].string));
if (parser->isExtractingSourceData() && parser->m_currentRuleDataStack->isEmpty() && parser->m_ruleSourceDataResult)
parser->addNewRuleToSourceTree(CSSRuleSourceData::createUnknown());
(yyval.rule) = 0;
}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 465 "generated/CSSGrammar.y"
    {
}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 467 "generated/CSSGrammar.y"
    {
}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 472 "generated/CSSGrammar.y"
    {

(yyval.rule) = 0;
}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 476 "generated/CSSGrammar.y"
    {
(yyval.rule) = 0;
}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 483 "generated/CSSGrammar.y"
    {
if ((yyvsp[(2) - (3)].rule) && parser->m_styleSheet)
parser->m_styleSheet->parserAppendRule((yyvsp[(2) - (3)].rule));
}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 510 "generated/CSSGrammar.y"
    {
parser->m_hadSyntacticallyValidCSSRule = true;
}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 519 "generated/CSSGrammar.y"
    { (yyval.ruleList) = 0; }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 520 "generated/CSSGrammar.y"
    {
(yyval.ruleList) = (yyvsp[(1) - (3)].ruleList);
if ((yyvsp[(2) - (3)].rule)) {
if (!(yyval.ruleList))
(yyval.ruleList) = parser->createRuleList();
(yyval.ruleList)->append((yyvsp[(2) - (3)].rule));
}
}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 553 "generated/CSSGrammar.y"
    {
parser->markRuleHeaderEnd();
parser->markRuleBodyStart();
}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 560 "generated/CSSGrammar.y"
    {
parser->markRuleHeaderStart(CSSRuleSourceData::IMPORT_RULE);
}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 566 "generated/CSSGrammar.y"
    {
(yyval.rule) = parser->createImportRule((yyvsp[(4) - (7)].string), (yyvsp[(6) - (7)].mediaList));
}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 569 "generated/CSSGrammar.y"
    {
(yyval.rule) = parser->createImportRule((yyvsp[(4) - (7)].string), (yyvsp[(6) - (7)].mediaList));
}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 572 "generated/CSSGrammar.y"
    {
(yyval.rule) = 0;
parser->popRuleData();
}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 576 "generated/CSSGrammar.y"
    {
(yyval.rule) = 0;
parser->popRuleData();
}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 580 "generated/CSSGrammar.y"
    {
(yyval.rule) = 0;
parser->popRuleData();
}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 587 "generated/CSSGrammar.y"
    {
parser->addNamespace((yyvsp[(3) - (6)].string), (yyvsp[(4) - (6)].string));
(yyval.rule) = 0;
}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 591 "generated/CSSGrammar.y"
    {
(yyval.rule) = 0;
}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 594 "generated/CSSGrammar.y"
    {
(yyval.rule) = 0;
}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 597 "generated/CSSGrammar.y"
    {
(yyval.rule) = 0;
}
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 603 "generated/CSSGrammar.y"
    { (yyval.string).clear(); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 604 "generated/CSSGrammar.y"
    { (yyval.string) = (yyvsp[(1) - (2)].string); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 613 "generated/CSSGrammar.y"
    {
(yyval.string) = (yyvsp[(1) - (2)].string);
}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 619 "generated/CSSGrammar.y"
    {
(yyval.valueList) = 0;
}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 622 "generated/CSSGrammar.y"
    {
(yyval.valueList) = (yyvsp[(3) - (4)].valueList);
}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 628 "generated/CSSGrammar.y"
    {


if ((yyvsp[(1) - (9)].mediaQueryRestrictor) != MediaQuery::None)
(yyval.mediaQueryExp) = parser->createFloatingMediaQueryExp("", 0);
else {
(yyvsp[(5) - (9)].string).lower();
(yyval.mediaQueryExp) = parser->createFloatingMediaQueryExp((yyvsp[(5) - (9)].string), (yyvsp[(7) - (9)].valueList));
}
}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 641 "generated/CSSGrammar.y"
    {
(yyval.mediaQueryExpList) = parser->createFloatingMediaQueryExpList();
(yyval.mediaQueryExpList)->append(parser->sinkFloatingMediaQueryExp((yyvsp[(1) - (1)].mediaQueryExp)));
}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 645 "generated/CSSGrammar.y"
    {
(yyval.mediaQueryExpList) = (yyvsp[(1) - (5)].mediaQueryExpList);
(yyval.mediaQueryExpList)->append(parser->sinkFloatingMediaQueryExp((yyvsp[(5) - (5)].mediaQueryExp)));
}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 652 "generated/CSSGrammar.y"
    {
(yyval.mediaQueryExpList) = parser->createFloatingMediaQueryExpList();
}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 655 "generated/CSSGrammar.y"
    {
(yyval.mediaQueryExpList) = (yyvsp[(3) - (3)].mediaQueryExpList);
}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 661 "generated/CSSGrammar.y"
    {
(yyval.mediaQueryRestrictor) = MediaQuery::None;
}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 664 "generated/CSSGrammar.y"
    {
(yyval.mediaQueryRestrictor) = MediaQuery::Only;
}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 667 "generated/CSSGrammar.y"
    {
(yyval.mediaQueryRestrictor) = MediaQuery::Not;
}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 673 "generated/CSSGrammar.y"
    {
(yyval.mediaQuery) = parser->createFloatingMediaQuery(parser->sinkFloatingMediaQueryExpList((yyvsp[(1) - (1)].mediaQueryExpList)));
}
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 677 "generated/CSSGrammar.y"
    {
(yyvsp[(3) - (4)].string).lower();
(yyval.mediaQuery) = parser->createFloatingMediaQuery((yyvsp[(1) - (4)].mediaQueryRestrictor), (yyvsp[(3) - (4)].string), parser->sinkFloatingMediaQueryExpList((yyvsp[(4) - (4)].mediaQueryExpList)));
}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 684 "generated/CSSGrammar.y"
    {
(yyval.mediaList) = parser->createMediaQuerySet();
}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 691 "generated/CSSGrammar.y"
    {
(yyval.mediaList) = parser->createMediaQuerySet();
(yyval.mediaList)->addMediaQuery(parser->sinkFloatingMediaQuery((yyvsp[(1) - (1)].mediaQuery)));
parser->updateLastMediaLine((yyval.mediaList));
}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 696 "generated/CSSGrammar.y"
    {
(yyval.mediaList) = (yyvsp[(1) - (4)].mediaList);
if ((yyval.mediaList)) {
(yyval.mediaList)->addMediaQuery(parser->sinkFloatingMediaQuery((yyvsp[(4) - (4)].mediaQuery)));
parser->updateLastMediaLine((yyval.mediaList));
}
}
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 703 "generated/CSSGrammar.y"
    {
(yyval.mediaList) = 0;
}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 709 "generated/CSSGrammar.y"
    {
parser->markRuleBodyStart();
}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 715 "generated/CSSGrammar.y"
    {
parser->markRuleHeaderStart(CSSRuleSourceData::MEDIA_RULE);
}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 721 "generated/CSSGrammar.y"
    {
parser->markRuleHeaderEnd();
}
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 727 "generated/CSSGrammar.y"
    {
(yyval.rule) = parser->createMediaRule((yyvsp[(4) - (10)].mediaList), (yyvsp[(9) - (10)].ruleList));
}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 730 "generated/CSSGrammar.y"
    {
(yyval.rule) = parser->createMediaRule(0, (yyvsp[(7) - (8)].ruleList));
}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 733 "generated/CSSGrammar.y"
    {
(yyval.rule) = 0;
parser->popRuleData();
}
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 740 "generated/CSSGrammar.y"
    {
(yyval.string) = (yyvsp[(1) - (2)].string);
}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 814 "generated/CSSGrammar.y"
    {
parser->markRuleHeaderStart(CSSRuleSourceData::KEYFRAMES_RULE);
}
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 820 "generated/CSSGrammar.y"
    {
(yyval.rule) = parser->createKeyframesRule((yyvsp[(4) - (10)].string), parser->sinkFloatingKeyframeVector((yyvsp[(9) - (10)].keyframeRuleList)));
}
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 831 "generated/CSSGrammar.y"
    { (yyval.keyframeRuleList) = parser->createFloatingKeyframeVector(); }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 832 "generated/CSSGrammar.y"
    {
(yyval.keyframeRuleList) = (yyvsp[(1) - (3)].keyframeRuleList);
if ((yyvsp[(2) - (3)].keyframe))
(yyval.keyframeRuleList)->append((yyvsp[(2) - (3)].keyframe));
}
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 840 "generated/CSSGrammar.y"
    {
(yyval.keyframe) = parser->createKeyframe((yyvsp[(1) - (6)].valueList));
}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 846 "generated/CSSGrammar.y"
    {
(yyval.valueList) = parser->createFloatingValueList();
(yyval.valueList)->addValue(parser->sinkFloatingValue((yyvsp[(1) - (1)].value)));
}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 850 "generated/CSSGrammar.y"
    {
(yyval.valueList) = (yyvsp[(1) - (5)].valueList);
if ((yyval.valueList))
(yyval.valueList)->addValue(parser->sinkFloatingValue((yyvsp[(5) - (5)].value)));
}
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 858 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).isInt = false; (yyval.value).fValue = (yyvsp[(1) - (2)].integer) * (yyvsp[(2) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 859 "generated/CSSGrammar.y"
    {
(yyval.value).id = 0; (yyval.value).isInt = false; (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER;
CSSParserString& str = (yyvsp[(1) - (1)].string);
if (str.equalIgnoringCase("from"))
(yyval.value).fValue = 0;
else if (str.equalIgnoringCase("to"))
(yyval.value).fValue = 100;
else
YYERROR;
}
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 872 "generated/CSSGrammar.y"
    {
parser->markRuleHeaderStart(CSSRuleSourceData::PAGE_RULE);
}
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 879 "generated/CSSGrammar.y"
    {
if ((yyvsp[(4) - (10)].selector))
(yyval.rule) = parser->createPageRule(parser->sinkFloatingSelector((yyvsp[(4) - (10)].selector)));
else {

parser->clearProperties();

(yyval.rule) = 0;
parser->popRuleData();
}
}
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 890 "generated/CSSGrammar.y"
    {
parser->popRuleData();
(yyval.rule) = 0;
}
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 894 "generated/CSSGrammar.y"
    {
parser->popRuleData();
(yyval.rule) = 0;
}
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 901 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setTag(QualifiedName(nullAtom, (yyvsp[(1) - (1)].string), parser->m_defaultNamespace));
(yyval.selector)->setForPage();
}
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 906 "generated/CSSGrammar.y"
    {
(yyval.selector) = (yyvsp[(2) - (2)].selector);
if ((yyval.selector)) {
(yyval.selector)->setTag(QualifiedName(nullAtom, (yyvsp[(1) - (2)].string), parser->m_defaultNamespace));
(yyval.selector)->setForPage();
}
}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 913 "generated/CSSGrammar.y"
    {
(yyval.selector) = (yyvsp[(1) - (1)].selector);
if ((yyval.selector))
(yyval.selector)->setForPage();
}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 918 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setForPage();
}
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 930 "generated/CSSGrammar.y"
    {
parser->startDeclarationsForMarginBox();
}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 932 "generated/CSSGrammar.y"
    {
(yyval.rule) = parser->createMarginAtRule((yyvsp[(1) - (7)].marginBox));
}
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 938 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::TopLeftCornerMarginBox;
}
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 941 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::TopLeftMarginBox;
}
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 944 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::TopCenterMarginBox;
}
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 947 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::TopRightMarginBox;
}
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 950 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::TopRightCornerMarginBox;
}
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 953 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::BottomLeftCornerMarginBox;
}
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 956 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::BottomLeftMarginBox;
}
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 959 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::BottomCenterMarginBox;
}
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 962 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::BottomRightMarginBox;
}
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 965 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::BottomRightCornerMarginBox;
}
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 968 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::LeftTopMarginBox;
}
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 971 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::LeftMiddleMarginBox;
}
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 974 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::LeftBottomMarginBox;
}
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 977 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::RightTopMarginBox;
}
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 980 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::RightMiddleMarginBox;
}
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 983 "generated/CSSGrammar.y"
    {
(yyval.marginBox) = CSSSelector::RightBottomMarginBox;
}
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 989 "generated/CSSGrammar.y"
    {
parser->markRuleHeaderStart(CSSRuleSourceData::FONT_FACE_RULE);
}
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 996 "generated/CSSGrammar.y"
    {
(yyval.rule) = parser->createFontFaceRule();
}
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 999 "generated/CSSGrammar.y"
    {
(yyval.rule) = 0;
parser->popRuleData();
}
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 1003 "generated/CSSGrammar.y"
    {
(yyval.rule) = 0;
parser->popRuleData();
}
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 1056 "generated/CSSGrammar.y"
    {
if ((yyvsp[(1) - (1)].selectorList)) {
parser->setReusableRegionSelectorVector((yyvsp[(1) - (1)].selectorList));
(yyval.selectorList) = parser->reusableRegionSelectorVector();
}
else
(yyval.selectorList) = 0;
}
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 1067 "generated/CSSGrammar.y"
    {
parser->markRuleHeaderStart(CSSRuleSourceData::REGION_RULE);
}
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 1073 "generated/CSSGrammar.y"
    {
if ((yyvsp[(4) - (10)].selectorList))
(yyval.rule) = parser->createRegionRule((yyvsp[(4) - (10)].selectorList), (yyvsp[(9) - (10)].ruleList));
else {
(yyval.rule) = 0;
parser->popRuleData();
}
}
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 1084 "generated/CSSGrammar.y"
    { (yyval.relation) = CSSSelector::DirectAdjacent; }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 1085 "generated/CSSGrammar.y"
    { (yyval.relation) = CSSSelector::IndirectAdjacent; }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 1086 "generated/CSSGrammar.y"
    { (yyval.relation) = CSSSelector::Child; }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 1090 "generated/CSSGrammar.y"
    { (yyval.integer) = (yyvsp[(1) - (1)].integer); }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 1091 "generated/CSSGrammar.y"
    { (yyval.integer) = 1; }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 1095 "generated/CSSGrammar.y"
    { (yyval.integer) = -1; }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 1096 "generated/CSSGrammar.y"
    { (yyval.integer) = 1; }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 1100 "generated/CSSGrammar.y"
    {
parser->markPropertyStart();
}
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 1106 "generated/CSSGrammar.y"
    {
parser->markRuleHeaderStart(CSSRuleSourceData::STYLE_RULE);
}
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 1112 "generated/CSSGrammar.y"
    {
parser->markRuleHeaderEnd();
}
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 1118 "generated/CSSGrammar.y"
    {
(yyval.rule) = parser->createStyleRule((yyvsp[(2) - (8)].selectorList));
}
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 1124 "generated/CSSGrammar.y"
    {
if ((yyvsp[(1) - (1)].selector)) {
(yyval.selectorList) = parser->reusableSelectorVector();
(yyval.selectorList)->shrink(0);
(yyval.selectorList)->append(parser->sinkFloatingSelector((yyvsp[(1) - (1)].selector)));
parser->updateLastSelectorLineAndPosition();
}
}
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 1132 "generated/CSSGrammar.y"
    {
if ((yyvsp[(1) - (4)].selectorList) && (yyvsp[(4) - (4)].selector)) {
(yyval.selectorList) = (yyvsp[(1) - (4)].selectorList);
(yyval.selectorList)->append(parser->sinkFloatingSelector((yyvsp[(4) - (4)].selector)));
parser->updateLastSelectorLineAndPosition();
} else
(yyval.selectorList) = 0;
}
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 1140 "generated/CSSGrammar.y"
    {
(yyval.selectorList) = 0;
}
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 1146 "generated/CSSGrammar.y"
    {
(yyval.selector) = (yyvsp[(1) - (2)].selector);
}
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 1152 "generated/CSSGrammar.y"
    {
(yyval.selector) = (yyvsp[(1) - (1)].selector);
}
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 1156 "generated/CSSGrammar.y"
    {
(yyval.selector) = (yyvsp[(1) - (1)].selector);
}
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 1160 "generated/CSSGrammar.y"
    {
(yyval.selector) = (yyvsp[(2) - (2)].selector);
if (!(yyvsp[(1) - (2)].selector))
(yyval.selector) = 0;
else if ((yyval.selector)) {
CSSParserSelector* end = (yyval.selector);
while (end->tagHistory())
end = end->tagHistory();
end->setRelation(CSSSelector::Descendant);
end->setTagHistory(parser->sinkFloatingSelector((yyvsp[(1) - (2)].selector)));
}
}
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 1172 "generated/CSSGrammar.y"
    {
(yyval.selector) = (yyvsp[(3) - (3)].selector);
if (!(yyvsp[(1) - (3)].selector))
(yyval.selector) = 0;
else if ((yyval.selector)) {
CSSParserSelector* end = (yyval.selector);
while (end->tagHistory())
end = end->tagHistory();
end->setRelation((yyvsp[(2) - (3)].relation));
end->setTagHistory(parser->sinkFloatingSelector((yyvsp[(1) - (3)].selector)));
}
}
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 1184 "generated/CSSGrammar.y"
    {
(yyval.selector) = 0;
}
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 1190 "generated/CSSGrammar.y"
    { (yyval.string).clear(); }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 1191 "generated/CSSGrammar.y"
    { static LChar star = '*'; (yyval.string).init(&star, 1); }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 1192 "generated/CSSGrammar.y"
    { (yyval.string) = (yyvsp[(1) - (2)].string); }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 1196 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setTag(QualifiedName(nullAtom, (yyvsp[(1) - (1)].string), parser->m_defaultNamespace));
}
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 1200 "generated/CSSGrammar.y"
    {
(yyval.selector) = (yyvsp[(2) - (2)].selector);
if ((yyval.selector))
parser->updateSpecifiersWithElementName(nullAtom, (yyvsp[(1) - (2)].string), (yyval.selector));
}
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 1205 "generated/CSSGrammar.y"
    {
(yyval.selector) = (yyvsp[(1) - (1)].selector);
if ((yyval.selector))
parser->updateSpecifiersWithElementName(nullAtom, starAtom, (yyval.selector));
}
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 1210 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setTag(parser->determineNameInNamespace((yyvsp[(1) - (2)].string), (yyvsp[(2) - (2)].string)));
}
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 1214 "generated/CSSGrammar.y"
    {
(yyval.selector) = (yyvsp[(3) - (3)].selector);
if ((yyval.selector))
parser->updateSpecifiersWithElementName((yyvsp[(1) - (3)].string), (yyvsp[(2) - (3)].string), (yyval.selector));
}
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 1219 "generated/CSSGrammar.y"
    {
(yyval.selector) = (yyvsp[(2) - (2)].selector);
if ((yyval.selector))
parser->updateSpecifiersWithElementName((yyvsp[(1) - (2)].string), starAtom, (yyval.selector));
}
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 1227 "generated/CSSGrammar.y"
    {
if ((yyvsp[(1) - (1)].selector)) {
(yyval.selectorList) = parser->createFloatingSelectorVector();
(yyval.selectorList)->append(parser->sinkFloatingSelector((yyvsp[(1) - (1)].selector)));
} else
(yyval.selectorList) = 0;
}
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 1234 "generated/CSSGrammar.y"
    {
if ((yyvsp[(1) - (5)].selectorList) && (yyvsp[(5) - (5)].selector)) {
(yyval.selectorList) = (yyvsp[(1) - (5)].selectorList);
(yyval.selectorList)->append(parser->sinkFloatingSelector((yyvsp[(5) - (5)].selector)));
} else
(yyval.selectorList) = 0;
}
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 1241 "generated/CSSGrammar.y"
    {
(yyval.selectorList) = 0;
}
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 1247 "generated/CSSGrammar.y"
    {
CSSParserString& str = (yyvsp[(1) - (1)].string);
if (parser->m_context.isHTMLDocument)
str.lower();
(yyval.string) = str;
}
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 1253 "generated/CSSGrammar.y"
    {
static LChar star = '*';
(yyval.string).init(&star, 1);
}
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 1260 "generated/CSSGrammar.y"
    {
(yyval.selector) = (yyvsp[(1) - (1)].selector);
}
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 1263 "generated/CSSGrammar.y"
    {
if (!(yyvsp[(2) - (2)].selector))
(yyval.selector) = 0;
else if ((yyvsp[(1) - (2)].selector))
(yyval.selector) = parser->updateSpecifiers((yyvsp[(1) - (2)].selector), (yyvsp[(2) - (2)].selector));
}
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 1269 "generated/CSSGrammar.y"
    {
(yyval.selector) = 0;
}
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 1275 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setMatch(CSSSelector::Id);
if (parser->m_context.mode == CSSQuirksMode)
(yyvsp[(1) - (1)].string).lower();
(yyval.selector)->setValue((yyvsp[(1) - (1)].string));
}
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 1282 "generated/CSSGrammar.y"
    {
if ((yyvsp[(1) - (1)].string)[0] >= '0' && (yyvsp[(1) - (1)].string)[0] <= '9') {
(yyval.selector) = 0;
} else {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setMatch(CSSSelector::Id);
if (parser->m_context.mode == CSSQuirksMode)
(yyvsp[(1) - (1)].string).lower();
(yyval.selector)->setValue((yyvsp[(1) - (1)].string));
}
}
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 1299 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setMatch(CSSSelector::Class);
if (parser->m_context.mode == CSSQuirksMode)
(yyvsp[(2) - (2)].string).lower();
(yyval.selector)->setValue((yyvsp[(2) - (2)].string));
}
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 1309 "generated/CSSGrammar.y"
    {
CSSParserString& str = (yyvsp[(1) - (2)].string);
if (parser->m_context.isHTMLDocument)
str.lower();
(yyval.string) = str;
}
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 1318 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setAttribute(QualifiedName(nullAtom, (yyvsp[(3) - (4)].string), nullAtom));
(yyval.selector)->setMatch(CSSSelector::Set);
}
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 1323 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setAttribute(QualifiedName(nullAtom, (yyvsp[(3) - (8)].string), nullAtom));
(yyval.selector)->setMatch((CSSSelector::Match)(yyvsp[(4) - (8)].integer));
(yyval.selector)->setValue((yyvsp[(6) - (8)].string));
}
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 1329 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setAttribute(parser->determineNameInNamespace((yyvsp[(3) - (5)].string), (yyvsp[(4) - (5)].string)));
(yyval.selector)->setMatch(CSSSelector::Set);
}
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 1334 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setAttribute(parser->determineNameInNamespace((yyvsp[(3) - (9)].string), (yyvsp[(4) - (9)].string)));
(yyval.selector)->setMatch((CSSSelector::Match)(yyvsp[(5) - (9)].integer));
(yyval.selector)->setValue((yyvsp[(7) - (9)].string));
}
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 1343 "generated/CSSGrammar.y"
    {
(yyval.integer) = CSSSelector::Exact;
}
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 1346 "generated/CSSGrammar.y"
    {
(yyval.integer) = CSSSelector::List;
}
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 1349 "generated/CSSGrammar.y"
    {
(yyval.integer) = CSSSelector::Hyphen;
}
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 1352 "generated/CSSGrammar.y"
    {
(yyval.integer) = CSSSelector::Begin;
}
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 1355 "generated/CSSGrammar.y"
    {
(yyval.integer) = CSSSelector::End;
}
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 1358 "generated/CSSGrammar.y"
    {
(yyval.integer) = CSSSelector::Contain;
}
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 1369 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setMatch(CSSSelector::PagePseudoClass);
(yyvsp[(2) - (2)].string).lower();
(yyval.selector)->setValue((yyvsp[(2) - (2)].string));
CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
if (type == CSSSelector::PseudoUnknown)
(yyval.selector) = 0;
}
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 1380 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setMatch(CSSSelector::PseudoClass);
(yyvsp[(2) - (2)].string).lower();
(yyval.selector)->setValue((yyvsp[(2) - (2)].string));
CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
if (type == CSSSelector::PseudoUnknown)
(yyval.selector) = 0;
}
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 1389 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setMatch(CSSSelector::PseudoElement);
(yyvsp[(3) - (3)].string).lower();
(yyval.selector)->setValue((yyvsp[(3) - (3)].string));

CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
if (type == CSSSelector::PseudoUnknown)
(yyval.selector) = 0;
}
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 1404 "generated/CSSGrammar.y"
    {
if ((yyvsp[(4) - (6)].selectorList)) {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setMatch(CSSSelector::PseudoClass);
(yyval.selector)->adoptSelectorVector(*parser->sinkFloatingSelectorVector((yyvsp[(4) - (6)].selectorList)));
(yyvsp[(2) - (6)].string).lower();
(yyval.selector)->setValue((yyvsp[(2) - (6)].string));
CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
if (type != CSSSelector::PseudoAny)
(yyval.selector) = 0;
} else
(yyval.selector) = 0;
}
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 1418 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setMatch(CSSSelector::PseudoClass);
(yyval.selector)->setArgument((yyvsp[(4) - (6)].string));
(yyval.selector)->setValue((yyvsp[(2) - (6)].string));
CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
if (type == CSSSelector::PseudoUnknown)
(yyval.selector) = 0;
}
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 1428 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setMatch(CSSSelector::PseudoClass);
(yyval.selector)->setArgument(String::number((yyvsp[(4) - (7)].integer) * (yyvsp[(5) - (7)].number)));
(yyval.selector)->setValue((yyvsp[(2) - (7)].string));
CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
if (type == CSSSelector::PseudoUnknown)
(yyval.selector) = 0;
}
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 1438 "generated/CSSGrammar.y"
    {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setMatch(CSSSelector::PseudoClass);
(yyval.selector)->setArgument((yyvsp[(4) - (6)].string));
(yyvsp[(2) - (6)].string).lower();
(yyval.selector)->setValue((yyvsp[(2) - (6)].string));
CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
if (type == CSSSelector::PseudoUnknown)
(yyval.selector) = 0;
else if (type == CSSSelector::PseudoNthChild ||
type == CSSSelector::PseudoNthOfType ||
type == CSSSelector::PseudoNthLastChild ||
type == CSSSelector::PseudoNthLastOfType) {
if (!isValidNthToken((yyvsp[(4) - (6)].string)))
(yyval.selector) = 0;
}
}
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 1456 "generated/CSSGrammar.y"
    {
if (!(yyvsp[(4) - (6)].selector) || !(yyvsp[(4) - (6)].selector)->isSimple())
(yyval.selector) = 0;
else {
(yyval.selector) = parser->createFloatingSelector();
(yyval.selector)->setMatch(CSSSelector::PseudoClass);

Vector<OwnPtr<CSSParserSelector> > selectorVector;
selectorVector.append(parser->sinkFloatingSelector((yyvsp[(4) - (6)].selector)));
(yyval.selector)->adoptSelectorVector(selectorVector);

(yyvsp[(2) - (6)].string).lower();
(yyval.selector)->setValue((yyvsp[(2) - (6)].string));
}
}
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 1474 "generated/CSSGrammar.y"
    {
(yyval.boolean) = (yyvsp[(1) - (1)].boolean);
}
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 1477 "generated/CSSGrammar.y"
    {
(yyval.boolean) = (yyvsp[(1) - (2)].boolean);
if ( (yyvsp[(2) - (2)].boolean) )
(yyval.boolean) = (yyvsp[(2) - (2)].boolean);
}
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 1482 "generated/CSSGrammar.y"
    {
(yyval.boolean) = (yyvsp[(1) - (1)].boolean);
}
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 1485 "generated/CSSGrammar.y"
    {
(yyval.boolean) = false;
}
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 1488 "generated/CSSGrammar.y"
    {
(yyval.boolean) = false;
}
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 1491 "generated/CSSGrammar.y"
    {
(yyval.boolean) = (yyvsp[(1) - (2)].boolean);
}
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 1494 "generated/CSSGrammar.y"
    {
(yyval.boolean) = (yyvsp[(1) - (2)].boolean);
}
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 1500 "generated/CSSGrammar.y"
    {
parser->markPropertyStart();
(yyval.boolean) = (yyvsp[(1) - (3)].boolean);
}
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 1504 "generated/CSSGrammar.y"
    {
(yyval.boolean) = false;
}
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 1507 "generated/CSSGrammar.y"
    {
(yyval.boolean) = false;
}
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 1510 "generated/CSSGrammar.y"
    {
parser->markPropertyStart();
(yyval.boolean) = false;
}
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 1514 "generated/CSSGrammar.y"
    {
(yyval.boolean) = false;
}
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 1517 "generated/CSSGrammar.y"
    {
parser->markPropertyStart();
(yyval.boolean) = (yyvsp[(1) - (4)].boolean);
if ((yyvsp[(2) - (4)].boolean))
(yyval.boolean) = (yyvsp[(2) - (4)].boolean);
}
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1523 "generated/CSSGrammar.y"
    {
parser->markPropertyStart();
(yyval.boolean) = (yyvsp[(1) - (4)].boolean);
}
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 1527 "generated/CSSGrammar.y"
    {
parser->markPropertyStart();
(yyval.boolean) = (yyvsp[(1) - (6)].boolean);
}
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 1534 "generated/CSSGrammar.y"
    {





(yyval.boolean) = false;

}
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 1544 "generated/CSSGrammar.y"
    {
(yyval.boolean) = false;
bool isPropertyParsed = false;
if ((yyvsp[(1) - (5)].id) && (yyvsp[(4) - (5)].valueList)) {
parser->m_valueList = parser->sinkFloatingValueList((yyvsp[(4) - (5)].valueList));
int oldParsedProperties = parser->m_parsedProperties.size();
(yyval.boolean) = parser->parseValue(static_cast<CSSPropertyID>((yyvsp[(1) - (5)].id)), (yyvsp[(5) - (5)].boolean));
if (!(yyval.boolean))
parser->rollbackLastProperties(parser->m_parsedProperties.size() - oldParsedProperties);
else
isPropertyParsed = true;
parser->m_valueList = nullptr;
}
parser->markPropertyEnd((yyvsp[(5) - (5)].boolean), isPropertyParsed);
}
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 1560 "generated/CSSGrammar.y"
    {
(yyval.boolean) = false;
}
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 1564 "generated/CSSGrammar.y"
    { 



parser->markPropertyEnd(false, false);
(yyval.boolean) = false;
}
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1572 "generated/CSSGrammar.y"
    { 

parser->markPropertyEnd(false, false);
(yyval.boolean) = false;
}
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1578 "generated/CSSGrammar.y"
    { 

(yyval.boolean) = false;
}
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1583 "generated/CSSGrammar.y"
    { 

parser->markPropertyEnd(false, false);
(yyval.boolean) = false;
}
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1589 "generated/CSSGrammar.y"
    { 

parser->markPropertyEnd(false, false);
(yyval.boolean) = false;
}
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1595 "generated/CSSGrammar.y"
    { 

(yyval.boolean) = false;
}
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1602 "generated/CSSGrammar.y"
    {
(yyval.id) = cssPropertyID((yyvsp[(1) - (2)].string));
}
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1608 "generated/CSSGrammar.y"
    { (yyval.boolean) = true; }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1609 "generated/CSSGrammar.y"
    { (yyval.boolean) = false; }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1613 "generated/CSSGrammar.y"
    {
(yyval.valueList) = parser->createFloatingValueList();
(yyval.valueList)->addValue(parser->sinkFloatingValue((yyvsp[(1) - (1)].value)));
}
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1617 "generated/CSSGrammar.y"
    {
(yyval.valueList) = (yyvsp[(1) - (3)].valueList);
if ((yyval.valueList)) {
if ((yyvsp[(2) - (3)].character)) {
CSSParserValue v;
v.id = 0;
v.unit = CSSParserValue::Operator;
v.iValue = (yyvsp[(2) - (3)].character);
(yyval.valueList)->addValue(v);
}
(yyval.valueList)->addValue(parser->sinkFloatingValue((yyvsp[(3) - (3)].value)));
}
}
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1630 "generated/CSSGrammar.y"
    {
(yyval.valueList) = 0;
}
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1633 "generated/CSSGrammar.y"
    {
(yyval.valueList) = 0;
}
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1636 "generated/CSSGrammar.y"
    {
(yyval.valueList) = 0;
}
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1642 "generated/CSSGrammar.y"
    {
(yyval.character) = '/';
}
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1645 "generated/CSSGrammar.y"
    {
(yyval.character) = ',';
}
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1648 "generated/CSSGrammar.y"
    {
(yyval.character) = 0;
}
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1654 "generated/CSSGrammar.y"
    { (yyval.value) = (yyvsp[(1) - (1)].value); }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1655 "generated/CSSGrammar.y"
    { (yyval.value) = (yyvsp[(2) - (2)].value); (yyval.value).fValue *= (yyvsp[(1) - (2)].integer); }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1656 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_STRING; }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1657 "generated/CSSGrammar.y"
    {
(yyval.value).id = cssValueKeywordID((yyvsp[(1) - (2)].string));
(yyval.value).unit = CSSPrimitiveValue::CSS_IDENT;
(yyval.value).string = (yyvsp[(1) - (2)].string);
}
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1663 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_DIMENSION; }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1664 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(2) - (3)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_DIMENSION; }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1665 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_URI; }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1666 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_UNICODE_RANGE; }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1667 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_PARSER_HEXCOLOR; }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1668 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = CSSParserString(); (yyval.value).unit = CSSPrimitiveValue::CSS_PARSER_HEXCOLOR; }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1669 "generated/CSSGrammar.y"
    {





}
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1677 "generated/CSSGrammar.y"
    {
(yyval.value) = (yyvsp[(1) - (1)].value);
}
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1680 "generated/CSSGrammar.y"
    {
(yyval.value) = (yyvsp[(1) - (1)].value);
}
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1683 "generated/CSSGrammar.y"
    {
(yyval.value) = (yyvsp[(1) - (1)].value);
}
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1686 "generated/CSSGrammar.y"
    { 
(yyval.value).id = 0; (yyval.value).unit = 0;
}
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1692 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).isInt = true; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1693 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).isInt = false; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1694 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PERCENTAGE; }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1695 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PX; }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1696 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_CM; }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1697 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_MM; }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1698 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_IN; }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1699 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PT; }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1700 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PC; }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1701 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_DEG; }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1702 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_RAD; }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1703 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_GRAD; }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1704 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_TURN; }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1705 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_MS; }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1706 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_S; }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1707 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_HZ; }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1708 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_KHZ; }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1709 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_EMS; }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1710 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSParserValue::Q_EMS; }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1711 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_EXS; }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1712 "generated/CSSGrammar.y"
    {
(yyval.value).id = 0;
(yyval.value).fValue = (yyvsp[(1) - (2)].number);
(yyval.value).unit = CSSPrimitiveValue::CSS_REMS;
if (parser->m_styleSheet)
parser->m_styleSheet->parserSetUsesRemUnits(true);
}
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1719 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_VW; }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1720 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_VH; }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1721 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_VMIN; }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1722 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_DPPX; }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1723 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_DPI; }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1724 "generated/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_DPCM; }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1728 "generated/CSSGrammar.y"
    {
CSSParserFunction* f = parser->createFloatingFunction();
f->name = (yyvsp[(1) - (5)].string);
f->args = parser->sinkFloatingValueList((yyvsp[(3) - (5)].valueList));
(yyval.value).id = 0;
(yyval.value).unit = CSSParserValue::Function;
(yyval.value).function = f;
}
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1736 "generated/CSSGrammar.y"
    {
CSSParserFunction* f = parser->createFloatingFunction();
f->name = (yyvsp[(1) - (4)].string);
f->args = parser->sinkFloatingValueList((yyvsp[(3) - (4)].valueList));
(yyval.value).id = 0;
(yyval.value).unit = CSSParserValue::Function;
(yyval.value).function = f;
}
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1744 "generated/CSSGrammar.y"
    {
CSSParserFunction* f = parser->createFloatingFunction();
f->name = (yyvsp[(1) - (4)].string);
CSSParserValueList* valueList = parser->createFloatingValueList();
f->args = parser->sinkFloatingValueList(valueList);
(yyval.value).id = 0;
(yyval.value).unit = CSSParserValue::Function;
(yyval.value).function = f;
}
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1753 "generated/CSSGrammar.y"
    {
CSSParserFunction* f = parser->createFloatingFunction();
f->name = (yyvsp[(1) - (3)].string);
f->args = nullptr;
(yyval.value).id = 0;
(yyval.value).unit = CSSParserValue::Function;
(yyval.value).function = f;
}
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1764 "generated/CSSGrammar.y"
    { (yyval.value) = (yyvsp[(1) - (1)].value); }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1765 "generated/CSSGrammar.y"
    {





}
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1772 "generated/CSSGrammar.y"
    { (yyval.value) = (yyvsp[(2) - (2)].value); (yyval.value).fValue *= (yyvsp[(1) - (2)].integer); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1776 "generated/CSSGrammar.y"
    {
(yyval.character) = '+';
}
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1779 "generated/CSSGrammar.y"
    {
(yyval.character) = '-';
}
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1782 "generated/CSSGrammar.y"
    {
(yyval.character) = '*';
}
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1785 "generated/CSSGrammar.y"
    {
(yyval.character) = '/';
}
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1791 "generated/CSSGrammar.y"
    {
if ((yyvsp[(3) - (6)].valueList)) {
(yyval.valueList) = (yyvsp[(3) - (6)].valueList);
CSSParserValue v;
v.id = 0;
v.unit = CSSParserValue::Operator;
v.iValue = '(';
(yyval.valueList)->insertValueAt(0, v);
v.iValue = ')';
(yyval.valueList)->addValue(v);
} else
(yyval.valueList) = 0;
}
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1806 "generated/CSSGrammar.y"
    {
(yyval.valueList) = parser->createFloatingValueList();
(yyval.valueList)->addValue(parser->sinkFloatingValue((yyvsp[(1) - (2)].value)));
}
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1810 "generated/CSSGrammar.y"
    {
if ((yyvsp[(1) - (3)].valueList) && (yyvsp[(2) - (3)].character)) {
(yyval.valueList) = (yyvsp[(1) - (3)].valueList);
CSSParserValue v;
v.id = 0;
v.unit = CSSParserValue::Operator;
v.iValue = (yyvsp[(2) - (3)].character);
(yyval.valueList)->addValue(v);
(yyval.valueList)->addValue(parser->sinkFloatingValue((yyvsp[(3) - (3)].value)));
} else
(yyval.valueList) = 0;

}
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1823 "generated/CSSGrammar.y"
    {
if ((yyvsp[(1) - (3)].valueList) && (yyvsp[(2) - (3)].character) && (yyvsp[(3) - (3)].valueList)) {
(yyval.valueList) = (yyvsp[(1) - (3)].valueList);
CSSParserValue v;
v.id = 0;
v.unit = CSSParserValue::Operator;
v.iValue = (yyvsp[(2) - (3)].character);
(yyval.valueList)->addValue(v);
(yyval.valueList)->extend(*((yyvsp[(3) - (3)].valueList)));
} else 
(yyval.valueList) = 0;
}
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1836 "generated/CSSGrammar.y"
    {
(yyval.valueList) = 0;
}
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1842 "generated/CSSGrammar.y"
    {
(yyval.valueList) = (yyvsp[(1) - (1)].valueList);
}
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1845 "generated/CSSGrammar.y"
    {
if ((yyvsp[(1) - (4)].valueList) && (yyvsp[(4) - (4)].valueList)) {
(yyval.valueList) = (yyvsp[(1) - (4)].valueList);
CSSParserValue v;
v.id = 0;
v.unit = CSSParserValue::Operator;
v.iValue = ',';
(yyval.valueList)->addValue(v);
(yyval.valueList)->extend(*((yyvsp[(4) - (4)].valueList)));
} else
(yyval.valueList) = 0;
}
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1860 "generated/CSSGrammar.y"
    {
CSSParserFunction* f = parser->createFloatingFunction();
f->name = (yyvsp[(1) - (5)].string);
f->args = parser->sinkFloatingValueList((yyvsp[(3) - (5)].valueList));
(yyval.value).id = 0;
(yyval.value).unit = CSSParserValue::Function;
(yyval.value).function = f;
}
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1868 "generated/CSSGrammar.y"
    {
YYERROR;
}
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1875 "generated/CSSGrammar.y"
    {
(yyval.string) = (yyvsp[(1) - (1)].string);
}
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1878 "generated/CSSGrammar.y"
    {
(yyval.string) = (yyvsp[(1) - (1)].string);
}
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1884 "generated/CSSGrammar.y"
    {
CSSParserFunction* f = parser->createFloatingFunction();
f->name = (yyvsp[(1) - (5)].string);
f->args = parser->sinkFloatingValueList((yyvsp[(3) - (5)].valueList));
(yyval.value).id = 0;
(yyval.value).unit = CSSParserValue::Function;
(yyval.value).function = f;
}
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1892 "generated/CSSGrammar.y"
    {
YYERROR;
}
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1900 "generated/CSSGrammar.y"
    {
(yyval.rule) = 0;
}
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1903 "generated/CSSGrammar.y"
    {
(yyval.rule) = 0;
}
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1909 "generated/CSSGrammar.y"
    {
(yyval.rule) = 0;
}
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1912 "generated/CSSGrammar.y"
    {
(yyval.rule) = 0;
}
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1918 "generated/CSSGrammar.y"
    {
(yyval.rule) = 0;
}
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1936 "generated/CSSGrammar.y"
    {
parser->invalidBlockHit();
}
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1939 "generated/CSSGrammar.y"
    {
parser->invalidBlockHit();
}
    break;



/* Line 1806 of yacc.c  */
#line 5060 "generated/CSSGrammar.cpp"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (parser, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (parser, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, parser);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, parser);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (parser, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, parser);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, parser);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 1949 "generated/CSSGrammar.y"


