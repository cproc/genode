/* Copyright 1995-2013 Mersenne Research, Inc. */
/* Author:  George Woltman */
/* Email: woltman@alum.mit.edu */

/* Include files needed by all ports */
#include <stdlib.h>
#include "gwnum.h"
#include <ctype.h>
#include <fcntl.h>
#include <math.h>
#include <memory.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* Common code */

#include "gwutil.h"

void RealOutputStr (int thread_num, char *buf)
{
	printf ("%s", buf);
}

/*----------------------------------------------------------------------
| Copyright 1995-2014 Mersenne Research, Inc.  All rights reserved
|
| This file contains routines and global variables that are common for
| all operating systems the program has been ported to.  It is included
| in one of the source code files of each port.  See common.h for the
| common #defines and common routine definitions.
|
| Commona contains information used only during setup
| Commonb contains information used only during execution
| Commonc contains information used during setup and execution
+---------------------------------------------------------------------*/

/****************************************************************************/
/*                Utility routines to output messages                       */
/****************************************************************************/

/* Output string to screen.  Prefix it with an optional timestamp. */

void OutputStr (
	int	thread_num,
	char	*buf)
{
	RealOutputStr (thread_num, buf);
}

/* Output string to both the screen and results file */

void OutputBoth (
	int	thread_num,
	char	*buf)
{
	OutputStr (thread_num, buf);
}

/* Output an out-of-memory error */

int OutOfMemory (
	int thread_num)
{
	OutputStr (thread_num, "Out of memory!\n");
	return -2;
}

/*----------------------------------------------------------------------
| Copyright 1995-2014 Mersenne Research, Inc.  All rights reserved
|
| This file contains routines and global variables that are common for
| all operating systems the program has been ported to.  It is included
| in one of the source code files of each port.
|
| Commona contains information used only during setup
| Commonb contains information used only during execution
| Commonc contains information used during setup and execution
+---------------------------------------------------------------------*/

/**************************************************************/
/*       Routines and globals dealing with stop codes         */
/*             and the write save files timer                 */
/**************************************************************/

/* This routine checks if the worker thread needs to be stopped for any */
/* reason whatsoever.  If the worker thread should stop, a stop reason */
/* is returned.  The routine is declared EXTERNC becasue it can be called */
/* by the C code in giants that does GCD. */

int stopCheck (
	int	thread_num)	/* Worker thread number */
{
	return (0);
}

/***************************************/
/* Routines to run a Lucas-Lehmer test */
/***************************************/

/* Structure for holding lucas setup data */

typedef struct {		/* Some of the data kept during LL test */
	gwhandle gwdata;	/* When we multithread the gwnum code, */
				/* gwsetup will return a handle */
	gwnum	lldata;		/* Number in the lucas sequence */
	unsigned long units_bit; /* Shift count */
} llhandle;

/* Prepare for running a Lucas-Lehmer test.  Caller must have already */
/* called gwinit. */

int lucasSetup (
	int	thread_num,	/* Worker thread number */
	unsigned long p,	/* Exponent to test */
	unsigned long fftlen,	/* Specific FFT length to use, or zero */
	llhandle *lldata)	/* Common LL data structure */
{
	int	res;

/* Init LL data structure */

	lldata->lldata = NULL;
	lldata->units_bit = 0;

/* Init the FFT code for squaring modulo 1.0*2^p-1.  NOTE: As a kludge for */
/* the benchmarking and timing code, an odd FFTlen sets up the 1.0*2^p+1 FFT code. */

	gwset_specific_fftlen (&lldata->gwdata, fftlen & ~1);
	if (fftlen & 1)
		res = gwsetup (&lldata->gwdata, 1.0, 2, p, 1);
	else
		res = gwsetup (&lldata->gwdata, 1.0, 2, p, -1);

/* If we were unable to init the FFT code, then print an error message */
/* and return an error code.  There is one exception, when we are doing */
/* a benchmark of all possible FFT implementations, do not print an error */
/* message. */

	if (res) {
		if (!lldata->gwdata.bench_pick_nth_fft) {
			char	buf[180];
			sprintf (buf, "Cannot initialize FFT code, errcode=%d\n", res);
			OutputBoth (thread_num, buf);
			gwerror_text (&lldata->gwdata, res, buf, sizeof (buf) - 1);
			strcat (buf, "\n");
			OutputBoth (thread_num, buf);
		}
		return -1;
	}

/* Allocate memory for the Lucas-Lehmer data (the number to square) */

	lldata->lldata = gwalloc (&lldata->gwdata);
	if (lldata->lldata == NULL) {
		gwdone (&lldata->gwdata);
		OutputStr (thread_num, "Error allocating memory for FFT data.\n");
		return -2;
	}
	return (0);
}

/* Clean up after running a Lucas-Lehmer test */

void lucasDone (
	llhandle *lldata)	/* Common LL data structure */
{

/* Free memory for the Lucas-Lehmer data */

	gwfree (&lldata->gwdata, lldata->lldata);

/* Cleanup the FFT code */

	gwdone (&lldata->gwdata);
}

/* Prepare for subtracting 2 from the squared result.  Also keep track */
/* of the location of the ever changing units bit. */

void lucas_fixup (
	llhandle *lldata,
	unsigned long p)	/* Exponent being tested */
{

/* We are about to square the number, the units bit position will double */

	lldata->units_bit <<= 1;
	if (lldata->units_bit >= p) lldata->units_bit -= p;

/* Tell gwnum code the value to subtract 2 from the squared result. */

	gwsetaddinatpowerofb (&lldata->gwdata, -2, lldata->units_bit);
}

/*********************/
/* Torture test code */
/*********************/

#define SELF1 "Test: Lucas-Lehmer iterations of M%ld using %s.\n"
char SELFFAIL1[] = "ERROR: ILLEGAL SUMOUT\n";

struct self_test_info {
	unsigned long p;
	unsigned long iters;
	unsigned long reshi;
};

#define MAX_SELF_TEST_ITERS	405
struct self_test_info SELF_TEST_DATA[MAX_SELF_TEST_ITERS] = {
{560000001, 100, 0x7F853A0A}, {420000001, 150, 0x89665E7E},
{280000001, 200, 0xC32CAD46}, {210000001, 300, 0x89823329},
{140000001, 400, 0x15EF4F24}, {110000001, 500, 0x893C9000},
{78643201, 400, 0x2D9C8904}, {78643199, 400, 0x7D469182},
{75497473, 400, 0x052C7FD8}, {75497471, 400, 0xCCE7495D},
{71303169, 400, 0x467A9338}, {71303167, 400, 0xBBF8B37D},
{68157441, 400, 0xBE71E616}, {68157439, 400, 0x93A71CC2},
{66060289, 400, 0xF296BB99}, {66060287, 400, 0x649EEF2A},
{62390273, 400, 0xBC8DFC27}, {62390271, 400, 0xDE7D5B5E},
{56623105, 400, 0x0AEBF972}, {56623103, 400, 0x1BA96297},
{53477377, 400, 0x5455F347}, {53477375, 400, 0xCE1C7F78},
{50331649, 400, 0x3D746AC8}, {50331647, 400, 0xE23F2DE6},
{49807361, 400, 0xB43EF4C5}, {49807359, 400, 0xA8BEB02D},
{47185921, 400, 0xD862563C}, {47185919, 400, 0x17281086},
{41943041, 400, 0x0EDA1F92}, {41943039, 400, 0xDE6911AE},
{39845889, 400, 0x43D8A96A}, {39845887, 400, 0x3D118E8F},
{37748737, 400, 0x38261154}, {37748735, 400, 0x22B34CD2},
{35651585, 400, 0xB0E48D2E}, {35651583, 400, 0xCC3340C6},
{34865153, 400, 0xD2C00E6C}, {34865151, 400, 0xFA644F69},
{33030145, 400, 0x83E5738D}, {33030143, 400, 0x6EDBC5B5},
{31195137, 400, 0xFF9591CF}, {31195135, 400, 0x04577C70},
{29884417, 400, 0xACC36457}, {29884415, 400, 0xC0FE7B1E},
{28311553, 400, 0x780EB8F5}, {28311551, 400, 0xE6D128C3},
{26738689, 400, 0x09DC45B0}, {26738687, 400, 0xDC7C074A},
{24903681, 400, 0xA482CF1E}, {24903679, 400, 0x4B3F5121},
{23592961, 400, 0xAFE3C198}, {23592959, 400, 0xCF9AD48C},
{20971521, 400, 0x304EC13B}, {20971519, 400, 0x9C4E157E},
{19922945, 400, 0x83FE36D9}, {19922943, 400, 0x9C60E7A2},
{18874369, 400, 0x83A9F8CB}, {18874367, 400, 0x5A6E22E0},
{17825793, 400, 0xF3A90A5E}, {17825791, 400, 0x6477CA76},
{17432577, 400, 0xCAB36E6A}, {17432575, 400, 0xB8F814C6},
{16515073, 400, 0x91EFCB1C}, {16515071, 400, 0xA0C35CD9},
{15597569, 400, 0x12E057AD}, {15597567, 400, 0xC4EFAEFD},
{14942209, 400, 0x1C912A7B}, {14942207, 400, 0xABA9EA6E},
{14155777, 400, 0x4A943A4E}, {14155775, 400, 0x00789FB9},
{13369345, 400, 0x27A041EE}, {13369343, 400, 0xA8B01A41},
{12451841, 400, 0x4DC891F6}, {12451839, 400, 0xA75BF824},
{11796481, 400, 0xFDD67368}, {11796479, 400, 0xE0237D19},
{10485761, 400, 0x15419597}, {10485759, 400, 0x154D473B},
{10223617, 400, 0x26039EB7}, {10223615, 400, 0xC9DFB1A4},
{9961473, 400, 0x3EB29644}, {9961471, 400, 0xE2AB9CB2},
{9437185, 400, 0x42609D65}, {9437183, 400, 0x77ED0792},
{8716289, 400, 0xCCA0C17B}, {8716287, 400, 0xD47E0E85},
{8257537, 400, 0x80B5C05F}, {8257535, 400, 0x278AE556},
{7798785, 400, 0x55A2468D}, {7798783, 400, 0xCF62032E},
{7471105, 400, 0x0AE03D3A}, {7471103, 400, 0xD8AB333B},
{7077889, 400, 0xC516359D}, {7077887, 400, 0xA23EA7B3},
{6684673, 400, 0xA7576F00}, {6684671, 400, 0x057E57F4},
{6422529, 400, 0xC779D2C3}, {6422527, 400, 0xA8263D37},
{6225921, 400, 0xB46AEB2F}, {6225919, 400, 0xD0A5FD5F},
{5898241, 400, 0xE46E76F9}, {5898239, 400, 0x29ED63B2},
{5505025, 400, 0x83566CC3}, {5505023, 400, 0x0B9CBE64},
{5242881, 400, 0x3CC408F6}, {5242879, 400, 0x0EA4D112},
{4980737, 400, 0x6A2056EF}, {4980735, 400, 0xE03CC669},
{4718593, 400, 0x87622D6B}, {4718591, 400, 0xF79922E2},
{4587521, 400, 0xE189A38A}, {4587519, 400, 0x930FF36C},
{4358145, 400, 0xDFEBF850}, {4358143, 400, 0xBB63D330},
{4128769, 400, 0xC0844AD1}, {4128767, 400, 0x25BDBFC3},
{3932161, 400, 0x7A525A7E}, {3932159, 400, 0xF30C9045},
{3735553, 400, 0xFAD79E97}, {3735551, 400, 0x005ED15A},
{3538945, 400, 0xDDE5BA46}, {3538943, 400, 0x15ED5982},
{3342337, 400, 0x1A6E87E9}, {3342335, 400, 0xECEEA390},
{3276801, 400, 0x3341C77F}, {3276799, 400, 0xACA2EE28},
{3112961, 400, 0x2BDF9D2B}, {3112959, 400, 0xA0AC8635},
{2949121, 400, 0x36EDB768}, {2949119, 400, 0x53FD5473},
{2785281, 400, 0x66816C94}, {2785279, 400, 0x059E8D6B},
{2654999, 400, 0x07EE900D}, {2621441, 400, 0x2BC1DACD},
{2621439, 400, 0xBCBA58F1}, {2653987, 400, 0xB005CACC},
{2651879, 400, 0x38DCD06B}, {2654003, 400, 0x1ED556E7},
{2620317, 400, 0x09DB64F8}, {2539613, 400, 0x4146EECA},
{2573917, 400, 0x939DA3B3}, {2359297, 400, 0x73A131F0},
{2359295, 400, 0x53A92203}, {2646917, 400, 0x71D4E5A2},
{2605473, 400, 0xE11637FC}, {2495213, 400, 0x89D80370},
{2540831, 400, 0x2CF01FBB}, {2654557, 400, 0x4106F46F},
{2388831, 400, 0xA508B5A7}, {2654777, 400, 0x9E744AA3},
{2584313, 400, 0x800E9A61}, {2408447, 400, 0x8C91E8AA},
{2408449, 400, 0x437ECC01}, {2345677, 400, 0x60AEE9C2},
{2332451, 400, 0xAB209667}, {2330097, 400, 0x3FB88055},
{2333851, 400, 0xFE4ECF19}, {2444819, 400, 0x56BF33C5},
{2555671, 400, 0x9DC03527}, {2654333, 400, 0xE81BCF40},
{2543123, 400, 0x379CA95D}, {2432123, 400, 0x5952676A},
{2321123, 400, 0x24DCD25F}, {2654227, 400, 0xAC3B7F2B},
{2329999, 400, 0xF5E902A5}, {2293761, 400, 0x9E4BBB8A},
{2293759, 400, 0x1901F07B}, {2236671, 400, 0x45EB162A},
{2193011, 400, 0x382B6E4B}, {2329001, 400, 0x4FF052BB},
{2327763, 400, 0x3B315213}, {2325483, 400, 0x0DC5165A},
{2323869, 400, 0xD220E27F}, {2315679, 400, 0xF650BE33},
{2004817, 400, 0xC2FF3440}, {2130357, 400, 0xC25804D8},
{2288753, 400, 0xA4DD9AAD}, {2266413, 400, 0x675257DB},
{2244765, 400, 0xC08FF487}, {2222517, 400, 0x1A128B22},
{2200339, 400, 0x0EB0E827}, {2328117, 400, 0x0A24673A},
{2329557, 400, 0x2E267692}, {2188001, 400, 0xD012AF6A},
{2166567, 400, 0x509BA41A}, {2144651, 400, 0x54CFC0E6},
{2122923, 400, 0xA47068E6}, {2100559, 400, 0xACFAB4E1},
{2088461, 400, 0xEA01E860}, {2066543, 400, 0x847DF0D0},
{2044767, 400, 0x04225888}, {2022823, 400, 0x6EA34B32},
{2328527, 400, 0xC55E3E05}, {2327441, 400, 0x207C8CEC},
{2326991, 400, 0x0A4F2ACD}, {2009987, 400, 0xE6A59DEF},
{1999999, 400, 0xD645A18F}, {1966081, 400, 0xB88828A1},
{1966079, 400, 0x5BD87C45}, {1998973, 400, 0xCBDD74F7},
{1997651, 400, 0x666B0CB1}, {1675001, 400, 0x50A94DB7},
{1977987, 400, 0x30D1CD1F}, {1955087, 400, 0x5B9426A4},
{1933071, 400, 0x23C1AF0B}, {1911957, 400, 0xF7699248},
{1899247, 400, 0x11C76E04}, {1877431, 400, 0xA3299B39},
{1855067, 400, 0x35243683}, {1833457, 400, 0xCF630DC0},
{1811987, 400, 0x7C7022EC}, {1799789, 400, 0xEFEC47B7},
{1777773, 400, 0x0F16E2D6}, {1755321, 400, 0x1AC5D492},
{1733333, 400, 0x5DA0555E}, {1711983, 400, 0xDC19DA8B},
{1699779, 400, 0x2B44914E}, {1677323, 400, 0x03D3980B},
{1995091, 400, 0x922E555B}, {1993041, 400, 0x0CA8451B},
{1991991, 400, 0xDFFB212D}, {1679779, 400, 0x51D75E0F},
{1684993, 400, 0x048BBCE8}, {1970009, 400, 0x646E0DFA},
{1957445, 400, 0xC8D244ED}, {1999997, 400, 0x5FC899D0},
{1998983, 400, 0x1CD518AA}, {1999007, 400, 0xA9DD8591},
{1674999, 400, 0xDB0169D8}, {1638401, 400, 0xD3F8A8C5},
{1638399, 400, 0xF270D8DD}, {1674997, 400, 0xC824EF15},
{1674551, 400, 0xD844AEAD}, {1674001, 400, 0x8F5EFA50},
{1345001, 400, 0x18EE2E2D}, {1655083, 400, 0x09B30DEE},
{1633941, 400, 0x0B87C8B1}, {1611557, 400, 0x6B57E48D},
{1599549, 400, 0x48EA38B2}, {1577771, 400, 0xCE84D9DC},
{1555947, 400, 0x6797EEF4}, {1533349, 400, 0xD6897409},
{1511861, 400, 0x8A8177AC}, {1499625, 400, 0x56BB6FB3},
{1477941, 400, 0xF3DD8ED3}, {1455931, 400, 0x31A222C7},
{1433069, 400, 0x28F01E1B}, {1411747, 400, 0x680C6E39},
{1399449, 400, 0xB7F01A54}, {1377247, 400, 0xE656F652},
{1355991, 400, 0xB2AA2819}, {1350061, 400, 0x31F9A728},
{1673881, 400, 0xA51D38E4}, {1672771, 400, 0x5474B6F9},
{1671221, 400, 0x2710DDEA}, {1670551, 400, 0x31FC3838},
{1660881, 400, 0x4C5B22C5}, {1650771, 400, 0x998F747B},
{1655001, 400, 0x164659A6}, {1674339, 400, 0xED2D23E2},
{1344999, 400, 0x158AA064}, {1310721, 400, 0x5694A427},
{1310719, 400, 0x258BDDE3}, {1344997, 400, 0x1D059D4F},
{1344551, 400, 0x60606AA3}, {1344001, 400, 0x9AC6AB36},
{1322851, 400, 0x3A000D0A}, {1300993, 400, 0x77CB0184},
{1288771, 400, 0x7431D9E2}, {1266711, 400, 0xB4BC4E8D},
{1244881, 400, 0x48BC9FF9}, {1222991, 400, 0x3F5FC39E},
{1200881, 400, 0xD5DF4944}, {1188441, 400, 0xD9D8968B},
{1166661, 400, 0xD4AB97F4}, {1144221, 400, 0x9940943B},
{1122001, 400, 0x647406B8}, {1100881, 400, 0x3AD40CE0},
{1088511, 400, 0xD578BB51}, {1066837, 400, 0x2F82BFBB},
{1044811, 400, 0x7C6EDDD1}, {1022991, 400, 0x6A1C2DD4},
{1000001, 400, 0x2879748F}, {1343881, 400, 0xB59E8006},
{1342771, 400, 0x87563FFE}, {1341221, 400, 0x29AD6127},
{1340551, 400, 0x17DB4ACB}, {1330881, 400, 0x9642F068},
{942079, 1000, 0xE528A9B0}, {974849, 1000, 0x79791EDB},
{983041, 1000, 0x29216C43}, {901121, 1000, 0x26C4E660},
{917503, 1000, 0x5F244685}, {933889, 1000, 0x62490F57},
{851967, 1000, 0x331AA906}, {860161, 1000, 0x41185F27},
{884735, 1000, 0x7BC7A661}, {802817, 1000, 0xA9645693},
{819199, 1000, 0x48AFB0A5}, {835585, 1000, 0x706437D3},
{753663, 1000, 0x99C43F31}, {778241, 1000, 0x1729A6C4},
{786431, 1000, 0x61080929}, {720897, 1000, 0x1E96863D},
{737279, 1000, 0x1B07A764}, {745473, 1000, 0x7BCE80AA},
{655359, 1000, 0x1107F161}, {659457, 1000, 0x589C16A4},
{688127, 1000, 0xD01E5A85}, {622593, 1000, 0x26F6FC8C},
{630783, 1000, 0x4DD2E603}, {638977, 1000, 0xC88F34B4},
{589823, 1000, 0x0290B60B}, {602113, 1000, 0xEFCD5BA8},
{614399, 1000, 0x6408F880}, {557057, 1000, 0xC30FE589},
{565247, 1000, 0xF4CA3679}, {573441, 1000, 0xF8F039AA},
{532479, 1000, 0x0072FE03}, {540673, 1000, 0xDA0E0D99},
{544767, 1000, 0x62443C6B}, {491521, 1000, 0x3F520DFA},
{516095, 1000, 0xA6BD9423}, {524289, 1000, 0xCD591388},
{466943, 1000, 0xE10EE929}, {471041, 1000, 0x18752F40},
{487423, 1000, 0x933FFF17}, {442369, 1000, 0xC22471C3},
{450559, 1000, 0x025B1320}, {458753, 1000, 0xE296CC00},
{417791, 1000, 0x080C803C}, {425985, 1000, 0xB2095F04},
{430079, 1000, 0x98B1EC61}, {393217, 1000, 0x26DD79ED},
{401407, 1000, 0x2F0F75F9}, {409601, 1000, 0xAEFAC2F8},
{372735, 1000, 0xCB6D00A2}, {376833, 1000, 0x915D5458},
{389119, 1000, 0x6188E38D}, {344065, 1000, 0x4D0C5089},
{360447, 1000, 0x84AC5CFD}, {368641, 1000, 0x72414364},
{319487, 1000, 0x24ED1BE9}, {327681, 1000, 0x3101106A},
{329727, 1000, 0x5BDB69AF}, {307201, 1000, 0x68536CD1},
{311295, 1000, 0x69778074}, {315393, 1000, 0x429D4950},
{286719, 1000, 0x1A31A686}, {294913, 1000, 0xF55727C6},
{301055, 1000, 0x33BDB242}, {272385, 1000, 0xEF6EC4B4},
{278527, 1000, 0x05530FD5}, {282625, 1000, 0x34A4E699},
{262143, 1000, 0xA9638844}, {266241, 1000, 0xE0969CED},
{270335, 1000, 0x14AD54BE}, {243713, 1000, 0xC19AEA91},
{245759, 1000, 0x7538BF0B}, {258049, 1000, 0x73F541AD},
{229375, 1000, 0x6E42B26A}, {233473, 1000, 0x1964F897},
{235519, 1000, 0x661BBC3F}, {215041, 1000, 0x04D5D2F0},
{221183, 1000, 0xA89E7764}, {225281, 1000, 0x20876BED},
{204799, 1000, 0xD20C2126}, {208897, 1000, 0x9D4DCF0E},
{212991, 1000, 0x1FF00E2A}, {194561, 1000, 0x6ED1CB70},
{196607, 1000, 0x3190D5F5}, {200705, 1000, 0xFAD28F5A},
{184319, 1000, 0x360EF08E}, {186369, 1000, 0x0F001482},
{188415, 1000, 0x86FCE4D6}, {164865, 1000, 0x4942B002},
{172031, 1000, 0xC5AF29DB}, {180225, 1000, 0x35D49D74},
{157695, 1000, 0x5422FACF}, {159745, 1000, 0xB5CD03A1},
{163839, 1000, 0x1CA6048E}, {150529, 1000, 0x7412F09C},
{153599, 1000, 0xA9FAAE69}, {155649, 1000, 0xA7B736AF},
{141311, 1000, 0x7A5D0730}, {143361, 1000, 0x580F4DC4},
{147455, 1000, 0x176B299A}, {135169, 1000, 0x65AC10A4},
{136191, 1000, 0xC4591D37}, {139265, 1000, 0xBCE1FC80},
{129023, 1000, 0xAFE1E7A8}, {131073, 1000, 0xC5AAB12F},
{133119, 1000, 0xDE51C35A}, {117761, 1000, 0x054A26F6},
{121855, 1000, 0x55AF2385}, {122881, 1000, 0x652827AC},
{112639, 1000, 0x6FA4DB24}, {114689, 1000, 0x0BBAF161},
{116735, 1000, 0xB85F0E8E}, {106497, 1000, 0xF833D925},
{107519, 1000, 0x80F177D8}, {110593, 1000, 0x1A56AA86},
{100351, 1000, 0x1DE12CE6}, {102401, 1000, 0x19F967B4},
{104447, 1000, 0xF9F3CDFD}
};

int selfTestInternal ()
{
	llhandle lldata;
	unsigned long k, limit;
	unsigned int i;
	char	buf[120];

	int thread_num = 1;
	unsigned long fftlen = 524288;
	unsigned int memory = 8;
	struct self_test_info *test_data = SELF_TEST_DATA;
	unsigned int test_data_count = MAX_SELF_TEST_ITERS;

/* Determine the range from which we'll choose an exponent to test. */

	limit = gwmap_fftlen_to_max_exponent (fftlen);

/* Loop testing various exponents from self test data array until */
/* time runs out */

	for (;;) {
		char	fft_desc[100];
		unsigned long p;
		unsigned int num_gwnums;
		gwnum	g;

/* Find next self test data entry to work on */

		for (i = 0; ; i++) {

/* Wrap in the self test data array */

			if (i == test_data_count) i = 0;

/* Now select the actual exponent */

			p = test_data[i].p;
			if (p > limit) continue;

/* The SSE2 carry propagation code gets into trouble if there are too */
/* few bits per FFT word!  Thus, we'll require at least 8 bits per */
/* word here.  Now that the number of iterations changes for each FFT */
/* length I'm raising the requirement to 10 bits to keep timings roughly */
/* equal. */

			if (p / fftlen < 10) continue;

/* We've found an exponent to test! */

			break;
		}

		sprintf(buf, "i = %d\n", i);
		OutputStr(thread_num, buf);

/* Now run Lucas setup, for extra safety double the maximum allowable */
/* sum(inputs) vs. sum(outputs) difference.  For faster detection of unstable */
/* systems, enable SUM(INPUTS) != SUM(OUTPUTS) checking on the first test. */
/* For a better variety of tests, enable SUM(INPUTS) != SUM(OUTPUTS) checking half the time. */

		gwinit (&lldata.gwdata);
		gwset_sum_inputs_checking (&lldata.gwdata, 1);
		gwset_num_threads (&lldata.gwdata, 1);
		lldata.gwdata.GW_BIGBUF = 0;
		lldata.gwdata.GW_BIGBUF_SIZE = 0;
		lucasSetup (thread_num, p, fftlen, &lldata);
		lldata.gwdata.MAXDIFF *= 2.0;

/* Output start message */

		gwfft_description (&lldata.gwdata, fft_desc);
		sprintf (buf, SELF1, p, fft_desc);
		OutputStr (thread_num, buf);

/* Determine how many gwnums we can allocate in the memory we are given */

		num_gwnums = 1;

/* Init data area with a pre-determined value */

restart_test:

		dbltogw (&lldata.gwdata, 4.0, lldata.lldata);
		g = lldata.lldata;

/* One Lucas-Lehmer test with error checking */

		gwsetnormroutine (&lldata.gwdata, 0, 1, 0);
		gwstartnextfft (&lldata.gwdata, 0);
		lucas_fixup (&lldata, p);
		gwsquare (&lldata.gwdata, g);

/* If the sum of the output values is an error (such as infinity) */
/* then raise an error. */

		if (gw_test_illegal_sumout (&lldata.gwdata))
			OutputBoth (thread_num, SELFFAIL1);

		//goto restart_test;
	}

	return (0);
}

/* Implemenet the rest of the OS-specific routines */

/* OSes that must poll for whether the ESC key was hit do it here. */
/* We use this opportunity to perform other miscellaneous tasks that */
/* can't be done any other way. */

void stopCheckCallback (
	int	thread_num)
{
}

int main (
	int	argc,
	char	*argv[])
{
	printf("mprime\n");

	StopCheckRoutine = stopCheck;
	OutputBothRoutine = OutputBoth;

	selfTestInternal();

	return (0);
}

