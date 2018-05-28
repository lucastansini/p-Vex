/*
Jeroen Email about parallel execution:
Basically what you need to do is turn the two benchmarks into a single program. You can probably get away with just
renaming the benchmark mains to <benchmark_name>_main(), and then either copypasting the two benchmark source files
into a new file or changing the compile script to compile and link both of them. Then you add a new main that does
approximately the following:

if (CR_CID == 0) {
  // Context 0:
  CR_CRR = 0x0011;
  benchmark_1_main();
} else {
  // Context 1:
  benchmark_2_main();
}
*/


#include "rvex.h"
#include "common.h"

/*Ucbqsort Benchmark code Area*/
unsigned char gQSortNum[1000] = {
  57, 126, 223, 44, 11, 138, 251, 232, 143, 86, 215, 60, 83, 30, 115, 48, 87, 46, 49, 164, 101, 198, 235, 72, 31,
  6, 57, 12, 227, 110, 157, 96, 25, 34, 191, 140, 43, 22, 37, 136, 175, 182, 183, 220, 115, 66, 173, 144, 137, 114,
  81, 68, 69, 102, 203, 168, 193, 102, 167, 172, 3, 14, 67, 64, 249, 62, 159, 20, 75, 74, 187, 216, 207, 22, 151,
  124, 109, 162, 51, 16, 151, 18, 143, 28, 37, 250, 171, 8, 95, 58, 135, 76, 221, 82, 221, 224, 217, 158, 127, 76,
  107, 170, 155, 200, 239, 138, 119, 228, 77, 2, 237, 176, 183, 178, 111, 132, 5, 166, 139, 104, 129, 218, 103, 20, 189,
  78, 3, 128, 185, 2, 95, 172, 139, 10, 123, 104, 241, 214, 169, 188, 211, 98, 13, 80, 41, 82, 177, 220, 229, 186,
  149, 200, 159, 134, 71, 116, 99, 18, 29, 224, 103, 162, 193, 12, 171, 150, 165, 248, 209, 202, 201, 92, 13, 62, 211,
  16, 247, 14, 209, 60, 59, 26, 181, 40, 191, 230, 39, 44, 125, 114, 195, 64, 135, 190, 225, 148, 53, 202, 197, 168,
  79, 106, 23, 4, 19, 34, 77, 144, 23, 146, 241, 156, 91, 122, 43, 136, 223, 70, 249, 52, 163, 46, 93, 96, 89,
  30, 255, 52, 21, 214, 229, 72, 111, 10, 9, 156, 205, 126, 147, 48, 55, 50, 239, 252, 133, 38, 11, 232, 1, 166,
  25, 108, 61, 206, 125, 0, 199, 126, 223, 44, 11, 118, 251, 24, 113, 170, 215, 60, 83, 30, 115, 48, 169, 210, 49,
  92, 101, 58, 21, 184, 225, 6, 199, 244, 227, 146, 99, 96, 25, 222, 65, 140, 213, 22, 219, 136, 175, 182, 73, 36,
  141, 190, 83, 112, 137, 114, 175, 188, 187, 102, 53, 168, 193, 154, 167, 172, 3, 242, 67, 64, 7, 62, 159, 236, 75,
  182, 187, 40, 49, 234, 105, 124, 109, 162, 205, 16, 105, 238, 143, 28, 37, 6, 85, 248, 161, 58, 121, 180, 221, 174,
  221, 32, 217, 98, 129, 76, 149, 170, 101, 200, 239, 118, 137, 28, 179, 254, 19, 80, 183, 178, 145, 124, 251, 166, 139,
  152, 129, 38, 153, 20, 189, 78, 3, 128, 71, 254, 95, 84, 139, 10, 123, 152, 241, 214, 169, 68, 211, 98, 13, 80,
  215, 82, 79, 36, 229, 70, 149, 56, 159, 134, 185, 140, 99, 18, 29, 224, 153, 94, 63, 244, 85, 150, 91, 248, 209,
  202, 55, 92, 243, 194, 211, 240, 247, 242, 209, 196, 197, 26, 75, 40, 191, 26, 39, 44, 131, 142, 195, 64, 121, 190,
  225, 148, 53, 54, 197, 88, 79, 106, 233, 252, 19, 34, 77, 144, 233, 146, 241, 100, 91, 134, 43, 136, 33, 70, 249,
  204, 163, 210, 163, 96, 89, 226, 1, 52, 235, 214, 27, 72, 111, 246, 247, 156, 51, 130, 109, 208, 55, 50, 17, 252,
  123, 218, 245, 232, 255, 90, 231, 108, 195, 50, 125, 0, 199, 130, 223, 212, 11, 138, 251, 232, 143, 86, 215, 196, 83,
  30, 141, 48, 87, 46, 207, 92, 155, 58, 235, 184, 31, 6, 199, 12, 29, 146, 157, 160, 231, 34, 191, 116, 213, 22,
  37, 136, 175, 74, 183, 36, 115, 66, 173, 144, 137, 142, 175, 188, 69, 102, 53, 168, 193, 102, 89, 172, 253, 242, 67,
  192, 7, 62, 159, 20, 181, 182, 187, 216, 207, 22, 105, 132, 109, 162, 205, 16, 151, 18, 113, 228, 37, 6, 85, 8,
  161, 58, 135, 76, 35, 174, 35, 224, 39, 158, 127, 180, 149, 86, 155, 200, 239, 118, 119, 28, 77, 254, 19, 176, 183,
  78, 145, 132, 5, 90, 117, 152, 127, 218, 153, 20, 67, 178, 3, 128, 185, 254, 95, 172, 139, 246, 123, 104, 15, 42,
  169, 68, 211, 98, 243, 80, 41, 174, 79, 36, 27, 186, 107, 56, 97, 134, 71, 116, 157, 238, 227, 32, 153, 162, 193,
  244, 171, 150, 165, 248, 209, 54, 55, 164, 13, 194, 45, 16, 247, 242, 47, 60, 59, 26, 75, 40, 65, 230, 217, 44,
  131, 142, 195, 192, 121, 190, 225, 108, 203, 202, 197, 168, 177, 150, 233, 252, 19, 34, 77, 144, 23, 110, 15, 100, 91,
  122, 213, 120, 223, 70, 7, 52, 93, 46, 93, 160, 167, 226, 255, 204, 21, 42, 229, 72, 111, 246, 9, 100, 205, 126,
  147, 48, 55, 206, 239, 4, 133, 218, 245, 232, 255, 166, 231, 108, 195, 50, 131, 0, 57, 130, 223, 44, 245, 118, 251,
  232, 113, 86, 215, 196, 83, 30, 141, 48, 87, 46, 207, 92, 155, 198, 235, 72, 31, 250, 57, 244, 29, 110, 99, 160,
  231, 34, 65, 116, 43, 22, 219, 136, 175, 74, 73, 220, 141, 190, 83, 112, 137, 142, 81, 68, 69, 154, 203, 88, 63,
  154, 89, 172, 3, 242, 67, 192, 249, 62, 159, 20, 181, 74, 69, 216, 49, 22, 105, 132, 109, 162, 205, 16, 151, 18,
  113, 228, 219, 250, 171, 8, 95, 58, 135, 180, 35, 82, 221, 224, 39, 98, 129, 180, 107, 86, 101, 200, 239, 118, 119,
  228, 179, 2, 237, 80, 183, 78, 111, 124, 5, 90, 117, 104, 127, 38, 153, 20, 67, 178, 253, 128, 185, 254, 95, 172,
  117, 10, 123, 152, 241, 42, 87, 188, 211, 98, 243, 80, 215, 82, 79, 36, 27, 186, 107, 200, 159, 122, 71, 140, 157,
  238, 29, 224, 153, 94, 63, 244, 85, 106, 91, 248, 47, 54, 201, 164, 243, 62, 211, 240, 247, 242, 47, 196, 197, 230,
  181, 216, 65, 26, 217, 44, 125, 114, 195, 192, 135, 190, 31, 148, 203, 202, 59, 88, 79, 150, 233, 252, 19, 34, 77,
  144, 233, 146, 15, 100, 165, 122, 213, 136, 223, 70, 249, 204, 93, 46, 93, 96, 167, 30, 1, 204, 21, 42, 27, 72
};
void QSORT(char *base, int n, int size, int (*compar) ());
int compare(char *n1, char *n2)
{
  return (*((unsigned char *) n1) - *((unsigned char *) n2));
}
int ucbqsort_main()
{
  //COMentado = 8-issue;
  unsigned char SortArr[1000];
  int j;
  memcpy(SortArr, gQSortNum, 1000);
  QSORT((char *) SortArr, (int) 1000, sizeof(unsigned char), compare);
  for (j = 1; j < 1000; j++)
    {
      if (SortArr[j] < SortArr[j - 1])
	{
	  rvex_fail("ucbqsort: failed\n");
	  return 1;
	}
    }
  rvex_succeed("ucbqsort: success\n");
  return 0;
}
static int (*qcmp) ();
static int qsz;
static int thresh;
static int mthresh;
static void qst(char *, char *);
void QSORT(base, n, size, compar)
     char *base;
     int n;
     int size;
     int (*compar) ();
{
  register char c, *i, *j, *lo, *hi;
  char *min, *max;
  if (n <= 1)
    return;
  qsz = size;
  qcmp = compar;
  thresh = qsz * 4;
  mthresh = qsz * 6;
  max = base + n * qsz;
  if (n >= 4)
    {
      qst(base, max);
      hi = base + thresh;
    }
  else
    {
      hi = max;
    }
  for (j = lo = base; (lo += qsz) < hi;)
    if ((*qcmp) (j, lo) > 0)
      j = lo;
  if (j != base)
    {
      for (i = base, hi = base + qsz; i < hi;)
	{
	  c = *j;
	  *j++ = *i;
	  *i++ = c;
	}
    }
  for (min = base; (hi = min += qsz) < max;)
    {
      while ((*qcmp) (hi -= qsz, min) > 0)
	;
      if ((hi += qsz) != min)
	{
	  for (lo = min + qsz; --lo >= min;)
	    {
	      c = *lo;
	      for (i = j = lo; (j -= qsz) >= hi; i = j)
		*i = *j;
	      *i = c;
	    }
	}
    }
}
static
void qst(base, max)
     char *base, *max;
{
  register char c, *i, *j, *jj;
  register int ii;
  char *mid, *tmp;
  int lo, hi;
  lo = max - base;
  do
    {
      mid = i = base + qsz * ((lo / qsz) >> 1);
      if (lo >= mthresh)
	{
	  j = ((*qcmp) ((jj = base), i) > 0 ? jj : i);
	  if ((*qcmp) (j, (tmp = max - qsz)) > 0)
	    {
	      j = (j == jj ? i : jj);
	      if ((*qcmp) (j, tmp) < 0)
		j = tmp;
	    }
	  if (j != i)
	    {
	      ii = qsz;
	      do
		{
		  c = *i;
		  *i++ = *j;
		  *j++ = c;
		}
	      while (--ii);
	    }
	}
      for (i = base, j = max - qsz;;)
	{
	  while (i < mid && (*qcmp) (i, mid) <= 0)
            i += qsz;
	  while (j > mid)
	    {
	      if ((*qcmp) (mid, j) <= 0)
		{
		  j -= qsz;
		  continue;
		}
	      tmp = i + qsz;
	      if (i == mid)
		{
		  mid = jj = j;
		}
	      else
		{
		  jj = j;
		  j -= qsz;
		}
	      goto swap;
	    }
	  if (i == mid)
	    {
	      break;
	    }
	  else
	    {
	      jj = mid;
	      tmp = mid = i;
	      j -= qsz;
	    }
	swap:
	  ii = qsz;
	  do
	    {
	      c = *i;
	      *i++ = *jj;
	      *jj++ = c;
	    }
	  while (--ii);
	  i = tmp;
	}
      i = (j = mid) + qsz;
      if ((lo = j - base) <= (hi = max - i))
	{
	  if (lo >= thresh)
            qst(base, j);
	  base = i;
	  lo = hi;
	}
      else
	{
	  if (hi >= thresh)
            qst(i, max);
	  max = j;
	}
    }
  while (lo >= thresh);
}



/*Ucbqsort Benchmark code Area*/
/*Jpeg Benchmark code Area*/

//#include <stdio.h>
int ncols = 240;
int nrows = 160;
unsigned long huffbits[] = {
  0xd3767676, 0xad0b7380, 0x08080822, 0x2235ad6e,
  0x575aecfc, 0xbd5a5667, 0x04045114, 0x104446b5,
  0xadc9eb25, 0xa38fa952, 0x5b000208, 0x38510446,
  0xb58dcaea, 0x64af87a7, 0x5a4b0009, 0x1d2852cd,
  0xf7a8888d, 0x635b93d4, 0x2c389a75, 0xdd6402b5,
  0x18d8e73f, 0x7241111b, 0x246bcf74, 0xab062e9c,
  0x25a406e5, 0x468d1c9a, 0xd6d10101, 0x30fa558b,
  0x0f4e26db, 0x408b2267, 0xa8b049b0, 0xb120892a,
  0x62746b1e, 0x16931968, 0x546e3db5, 0x7be2864d,
  0x228c324b, 0x5b45d89d, 0x18cc3d06, 0xc56c523a,
  0x72c48f8d, 0x6dd92ac3, 0x2be0baec, 0x5e806e15,
  0xf48ad8a3, 0x2b39f0ac, 0x8cb13bea, 0xc76228ed,
  0xae2ef898, 0x37c82e0a, 0x15992312, 0x44b6f950,
  0x068dc5de, 0x56e15d75, 0x7b8a0e8e, 0xab902c5b,
  0x6c802223, 0x71f70306, 0xebeb5c51, 0xfa2dc463,
  0xc6e82922, 0x88c6b9b9, 0x1b61856e, 0x4ad715da,
  0xf2c70655, 0x44b565c9, 0x22a22351, 0xd95ae187,
  0x6a5ab766, 0xd591ad63, 0x6ae75957, 0xb2410421,
  0xb98baa18, 0xb626abb3, 0xa6a8d630, 0x4c49e41b,
  0x2587b4b1, 0x0985a418, 0xd3cf26ea, 0x8232363b,
  0x04be46d6, 0x59b32aa4, 0x78ba018f, 0x3dad9731,
  0x11cd1b1e, 0x6d6d1188, 0x3992b513, 0x3f4c3265,
  0xdab51800, 0xe6b33e86, 0xa35a8ae4, 0x51133f5a,
  0x456bdcaa, 0x39051618, 0x71762263, 0x495b3306,
  0xe78a15ad, 0x0fddc47d, 0xcd5af13a, 0xd0e957df,
  0xeb87fbb5, 0x66736c9f, 0x4acfbd66, 0xb3ef59f7,
  0xacd66b35, 0x9acd66aa, 0xab155445, 0x11468d1a,
  0x35764993, 0x93c02714, 0xa2b15ac8, 0xfdc467fd,
  0xafe95071, 0x32fd697a, 0x55f7fad5, 0xfa5591ff,
  0x465e6b35, 0x9acd66b3, 0x559acd55, 0x55555551,
  0xa3468d1a, 0xbbff587e, 0xa694d2f3, 0x5acaffa2,
  0x29ff6c7f, 0x23519fde, 0xa7d697a5, 0x5f7de4e3,
  0xb5591ff4, 0x71f5acd6, 0x6b359aaa, 0xcd66b70a,
  0xf31738c8, 0xacd55562, 0xb14451a3, 0x468d1abc,
  0xff5bf89a, 0x0691b9ad, 0x5c66c4ff, 0xbe293891,
  0x7eb4a78a, 0xbefe0356, 0x27f71f8d, 0x66aaab35,
  0x52cc2307, 0xb9a96e24, 0x3c838fa1, 0xa33bff78,
  0xfe74b331, 0x3d6a3bb2, 0x8df31245, 0x4522c881,
  0x94f1558a, 0xac511468, 0xd1a346af, 0x7fd6fe26,
  0xa81ad44e, 0x6c5fea29, 0x7ef2e7d6, 0x93a0abdf,
  0xbab5627f, 0x74df5aaa, 0xaaab8b9d, 0x84a2f5ee,
  0x69a5e702, 0x9df26b35, 0x19c1a279, 0xab7b830c,
  0x808fba7a, 0x8a8019d3, 0x746370f5, 0xaf224c7d,
  0xdfd6bc89, 0x3fb87f3a, 0xf225fee7, 0xeb5f6797,
  0xfb9fad1b, 0x797fb86b, 0xecf3138d, 0x847b9e94,
  0x2c723fd6, 0x60ffb946, 0xd1039437, 0x0a1bd303,
  0x3fce9ec9, 0x51493313, 0xec13ffaf, 0x57e009ce,
  0x0e467d2a, 0xaafb9b39, 0x2810587d, 0x693eed5e,
  0x7fab5fad, 0x589f91be, 0xb55559a7, 0x6da85bd0,
  0x53ca0b13, 0xdcd7534c, 0x2b142b1c, 0x74aad22e,
  0x19a27b72, 0x46d1c814, 0x608bba7f, 0xe3c7fc6b,
  0xc98bfe79, 0xffe3c7fc, 0x6bca8ffe, 0x798fd6bc,
  0xb5fee2ff, 0xdf35b107, 0xf027fdf2, 0x2b083b20,
  0xfc056f51, 0xfc407e35, 0xe67a3feb, 0x4ccc47de,
  0x27f1abef, 0xf5bf8d0a, 0xabce6d25, 0xff76bd2a,
  0x33f28abc, 0xe621f5ab, 0x1e8ff5aa, 0xaaaba38b,
  0x76fa51eb, 0x51a64579, 0x59a10e3b, 0x50828c40,
  0x0a9531c8, 0xab072972, 0x3048c8c7, 0x148c7382,
  0x73f5a62a, 0xa3240fca, 0x9a7841e7, 0xf957da2d,
  0xfdbfef9a, 0x13c27a7f, 0x2a6b9854, 0xf24fe55f,
  0x6b8719c9, 0xfca83065, 0x0c3a119a, 0x6e957ffe,
  0xbbf3aaab, 0x9ff8f697, 0xfdd35d79, 0xa8cfcb57,
  0x5cc3f8d5, 0x91fbff85, 0x66b35553, 0x2ef89947,
  0x7145487d, 0xa7ae6861, 0x16bcddbd, 0x54d24c8d,
  0x45940a69, 0x63ce3352, 0x90ca715a, 0x747990b9,
  0xe80714bf, 0x7855c1c2, 0xa9c719a9, 0x41df9c81,
  0x5d08f98e, 0x3dea35dd, 0xc8e84f5c, 0x53c6a460,
  0xf14f091c, 0x839a807e, 0xe23cff74, 0x5374ad43,
  0xfd77e755, 0x53f36f20, 0xff64d76a, 0x8cfca2ae,
  0x7fd49fad, 0x591f99ff, 0x0aaacd54, 0xcc5530bc,
  0x13de8aab, 0xc809e5bb, 0x9a68f70e, 0x28c4d9e4,
  0x935e5e1f, 0xd2a41903, 0x06821049, 0x03f4a642,
  0xcbc8009a, 0xb3184207, 0x6e287515, 0x70bbd000,
  0x71cd3200, 0xd8624834, 0xd10ce060, 0x63be2a32,
  0x41da016f, 0x514e3804, 0xfca7fdae, 0x2a4e1720,
  0x1cfa0a80, 0xe6043eaa, 0x29ab5118, 0x941fad55,
  0x4bcc2ffe, 0xe9fe55d8, 0xd447e415, 0x71fea4d5,
  0x99f9dbe9, 0x59acd554, 0x8bb97029, 0x94231a56,
  0xaa6601b9, 0x34c578c1, 0xa038a90f, 0xa55ae029,
  0xf53cd0ea, 0x29c12054, 0xd1bbf017, 0x8a8ede40,
  0xe771dcbd, 0x813d682b, 0xaa12a837, 0x7a66bca7,
  0x700caa49, 0xf4cf4a78, 0xa43c2c78, 0xfc45440a,
  0xc4808c10, 0x318a35a9, 0x7faf1554, 0xfcc6c3fd,
  0x9346a13f, 0x20ab8ff5, 0x2d5687f7, 0xadf4aaaa,
  0xaa94618f, 0x34b5bb02, 0x9b696c9c, 0x51c678c5,
  0x06e2911a, 0x5976afd6, 0xbcb11baa, 0x8feed0eb,
  0x5dab8aaa, 0xaa346b53, 0xff5c2aa9, 0xbee9fa57,
  0xad407318, 0xa9ffd4b7, 0xd2ad3fd6, 0x9fa5566a,
  0xa914bb85, 0x1d49abe8, 0x1a090839, 0x20f43eb4,
  0x0e2b20f1, 0x5e58ea00, 0xad9ed8fa, 0x5741d6ad,
  0x118ee903, 0x32f38e0e, 0x2a4fbc0f, 0x7c550aaa,
  0xaaa268d6, 0xa9feb96b, 0x359a347a, 0x9ab73fbb,
  0x152ffaa6, 0xfa55affa, 0xefc2aaaa, 0x34691c22,
  0x0c9356f6, 0x6213bd8e, 0xe6c74f4a, 0xba884c85,
  0x1bf0f6a9, 0xe1781f0c, 0x38ec681a, 0x0f4cf48a,
  0x64638e07, 0xad5bae20, 0x603b1a90, 0x90c838c1,
  0x1542aab3, 0x52b158c9, 0x5193e951, 0x48644dc5,
  0x76d1344d, 0x6a9feb57, 0xf1aacd53, 0x7de3f5ab,
  0x63fbb152, 0x7fab6fa5, 0x5b7fae1f, 0x4aaa8e36,
  0x9182a0c9, 0x356d02c1, 0x1e072c7e, 0xf1f5aa71,
  0xebc8a922, 0x59176b00, 0x455ce9ef, 0x192d1f2b,
  0xe83a8a21, 0x8706b1eb, 0x50aed8c7, 0xbf3509f9,
  0x187bd480, 0xe50fa566, 0x81e2b359, 0xacd1359a,
  0x26844cc3, 0x2197f3ad, 0x590abc67, 0x2a739e87,
  0xe9559a1d, 0x69fefb7d, 0x6ad8feec, 0x53ffab6f,
  0xa55b9fdf, 0x2d66aded, 0xde73f2f0, 0xa3a93504,
  0x0902e147, 0x3dc9ea6b, 0x35d0f14e, 0xdc570466,
  0x88cd4f6a, 0x928ce30d, 0xea28c056, 0x6db210a0,
  0x7735226d, 0xc11f74f4, 0xa84e15be, 0xb4ec0aaf,
  0x6f6359a0, 0x78acd6ea, 0xf9bd0fe5, 0x586feeb7,
  0xe55e5ca7, 0xfe59bffd, 0xf268cabf, 0xde1f9d46,
  0x0344ac00, 0x395eb5ab, 0x758bf1fe, 0x95540d4b,
  0xfeb5fea6, 0xad8feec5, 0x37dc6fa5, 0x41feb96a,
  0xcecccb87, 0x9384ec3d, 0x6800a000, 0x300551cf,
  0xad31c753, 0x58e7ad36, 0x55b22b86, 0x191441ad,
  0x525125d1, 0x5038418f, 0xc6aca6dd, 0x6c1187dd,
  0xe0d20c6f, 0x02a56dc1, 0x0e31cf43, 0x44f150ba,
  0x0705f180, 0x3b8cd09a, 0x12c02c71, 0x9fa633f9,
  0x1af3147f, 0x747d4018, 0xa79d48c0, 0x9769f518,
  0xa8ee5570, 0x243bc67e, 0xf6cc63eb, 0x4d25baf0,
  0xc53e8456, 0x74f1fc30, 0x0ff808af, 0x3edcb6d4,
  0x75f60056, 0xb03062ff, 0x817f4acd, 0x66b3537f,
  0xae7ff78d, 0x5b7dca3f, 0x74fd2b49, 0xb132b89e,
  0x40422fdd, 0x1eb42b20, 0x7535b97d, 0x45641e86,
  0x9b06b233, 0xd69f9622, 0x836d39a7, 0x3b632c3e,
  0x957a9b64, 0xcf4ab16c, 0x48cbea33, 0x49d49f61,
  0x5201b41a, 0x6e95da8a, 0x96200a8a, 0x5fb2a709,
  0x9c9e7d6a, 0x1bc8a519, 0x071f5a77, 0x42558c9b,
  0x71e87afe, 0x146e211d, 0x587e028d, 0xdc23f89b,
  0xf2a92e2d, 0x9d70dbc8, 0xad69a33e, 0x488d9c81,
  0xbbef7e15, 0x9acd54df, 0xebdffde3, 0x56fc2541,
  0x1f9d204e, 0xddfe9512, 0x8440aa30, 0x074a67c7,
  0x03ad1392, 0x73cd0231, 0x83592a78, 0x342538c1,
  0xa246ec8e, 0xf4c73cd3, 0x7634e4e0, 0x03d2b500,
  0x36123a83, 0x5667fd24, 0x73c1159d, 0xa327a77a,
  0x949d808e, 0x99a3d2bb, 0x520249c7, 0x6e6a6ff5,
  0x7f88a831, 0x93f2e3e9, 0x46874a34, 0x6b53ff96,
  0x7f8ff4a0, 0x6b359a9f, 0xfd7bffbc, 0x6adfeed5,
  0x9c5b100e, 0xe7934cfd, 0x8566b359, 0xace2b347,
  0x8a3cae45, 0x019522b3, 0x918357c8, 0x42b77cd5,
  0xb1c5c211, 0x4c01041e, 0x86997108, 0x0beb4687,
  0x4a8fef1a, 0x9394c75a, 0x5524ed07, 0xe6eb81da,
  0x973b46ee, 0xbdebb51a, 0x35a9ffcb, 0x3fc7fa52,
  0xc27bb0fc, 0x28429ea6, 0x82c63f87, 0xf3a30db9,
  0x3930c64f, 0xa9514891, 0x29f96341, 0xf4514240,
  0x091de836, 0x79acd035, 0x555d4628, 0x77a1f7ff,
  0x0a7f9643, 0xe86ae977, 0x447b9140, 0xec901ee0,
  0xe681ca02, 0x2bcc50a5, 0x79ce68c9, 0xed42438e,
  0x94252a73, 0x8af304a8, 0x7660b7a5, 0x407f78cc,
  0xa195fa36, 0x78c53124, 0xe4f5a06a, 0x89ad4bac,
  0x7f8ff4a0
};
static unsigned int lastlong = 0;
static int bitsleft = 0;
static unsigned long *nextlong = huffbits;


int getbit()
{
  int bit;

  if (--bitsleft < 0)
    {
      if (nextlong >= &huffbits[sizeof(huffbits)])
	lastlong = 0;
      else
	{
	  lastlong = *nextlong++;
	  bitsleft = 31;
	}
    }
  bit = (lastlong & 0x80000000) != 0;
  lastlong <<= 1;
  return (bit);
}
unsigned char val_dc_lum[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
			      0x07, 0x08, 0x09, 0x0A, 0x0B};
unsigned char val_ac_lum[] = {0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21,
			      0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07, 0x22, 0x71, 0x14,
			      0x32, 0x81, 0x91, 0xa1, 0x08, 0x23, 0x42, 0xb1, 0xc1, 0x15,
			      0x52, 0xd1, 0xf0, 0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a,
			      0x16, 0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28, 0x29,
			      0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44,
			      0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x53, 0x54, 0x55, 0x56,
			      0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
			      0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a,
			      0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x92, 0x93,
			      0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3,
			      0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3,
			      0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
			      0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3,
			      0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2, 0xe3,
			      0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3,
			      0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa};
unsigned char zz_tbl[32] = {
  0, 1, 8, 16, 9, 2, 3, 10, 17, 24, 32, 25, 18, 11, 4, 5, 12, 19, 26, 33, 40, 48, 41, 34, 27,
  20, 13, 6, 7, 14, 21, 28
};
int mincode_dc[] = {
  0x0,
  0x0,
  0x0,
  0x2,
  0xe,
  0x1e,
  0x3e,
  0x7e,
  0xfe,
  0x1fe
};
int maxcode_dc[] = {
  0x0,
  -1,
  0x0,
  0x6,
  0xe,
  0x1e,
  0x3e,
  0x7e,
  0xfe,
  0x1ff
};
unsigned char valptr_dc[] = {
  0,
  0,
  0,
  1,
  6,
  7,
  8,
  9,
  10,
  11
};
int mincode_ac[] = {
  0x0,
  0x0,
  0x0,
  0x4,
  0xa,
  0x1a,
  0x3a,
  0x78,
  0xf8,
  0x1f6,
  0x3f6,
  0x7f6,
  0xff4,
  0x0,
  0x0,
  0x7fc0,
  0xff82
};
int maxcode_ac[] = {
  0x0,
  -1,
  0x1,
  0x4,
  0xc,
  0x1c,
  0x3b,
  0x7b,
  0xfa,
  0x1fa,
  0x3fa,
  0x7f9,
  0xff7,
  -1,
  -1,
  0x7fc0,
  0xffff
};
unsigned char valptr_ac[] = {
  0,
  0,
  0,
  2,
  3,
  6,
  9,
  11,
  15,
  18,
  23,
  28,
  32,
  0,
  0,
  36,
  37
};

void huff_dc_dec(int *retval)
{
  int ivariable, s, l, p, code;

  l = 1;
  code = getbit();
  while (code > maxcode_dc[l])
    {
      l++;
      code = (code << 1) + getbit();
    }
  p = valptr_dc[l];
  p = p + code - mincode_dc[l];
  s = val_dc_lum[p];
  *retval = 0;
  for (ivariable = 0; ivariable < s; ivariable++)
    *retval = (*retval << 1) + getbit();
  ivariable = 1 << (s - 1);
  while (*retval < ivariable)
    {
      ivariable = (-1 << s) + 1;
      *retval = *retval + ivariable;
    }
}

void huff_ac_dec(short *data)
{
  int ivariable, j, icnt, ns, n, s, l, p, code, dindex;
  int temp, data_zz[64];

  dindex = 0;
  while (dindex < 63)
    {
      l = 1;
      code = getbit();
      while (code > maxcode_ac[l])
	{
	  l = l + 1;
	  code = (code << 1) + getbit();
	}
      p = valptr_ac[l];
      p = p + code - mincode_ac[l];
      ns = val_ac_lum[p];
      s = ns & 0x0f;
      n = ns >> 4;
      if (ns != 0x00)
	{
	  if (ns != 0xF0)
	    {
	      for (ivariable = 0; ivariable < n; ivariable++)
		{
		  data_zz[dindex] = 0;
		  dindex = dindex + 1;
		}
	      temp = 0;
	      for (ivariable = 0; ivariable < s; ivariable++)
		temp = (temp << 1) + getbit();
	      ivariable = 1 << (s - 1);
	      while (temp < ivariable)
		{
		  ivariable = (-1 << s) + 1;
		  temp = temp + ivariable;
		}
	      data_zz[dindex] = temp;
	      dindex = dindex + 1;
	    }
	  else
	    {
	      for (ivariable = 0; ivariable < 16; ivariable++)
		{
		  data_zz[dindex] = 0;
		  dindex = dindex + 1;
		}
	    }
	}
      else
	{
	  icnt = 63 - dindex;
	  for (ivariable = 0; ivariable < icnt; ivariable++)
	    {
	      data_zz[dindex] = 0;
	      dindex = dindex + 1;
	    }
	}
    }
  for (ivariable = 0; ivariable < 31; ivariable++)
    {
      *(data + zz_tbl[ivariable + 1]) = data_zz[ivariable];
    }
  for (j = 31, ivariable = 31; ivariable > 0; ivariable--, j++)
    {
      *(data + 63 - zz_tbl[ivariable]) = data_zz[j];
    }
  *(data + 63) = data_zz[62];
}

fast_idct_8(short *in, int stride)
{
  int tmp10, tmp11, tmp12, tmp13;
  int tmp20, tmp21, tmp22, tmp23;
  int tmp30, tmp31;
  int tmp40, tmp41, tmp42, tmp43;
  int tmp50, tmp51, tmp52, tmp53;
  int in0, in1, in2, in3, in4, in5, in6, in7;
  int ivariable, j;
  in0 = in[0];
  in1 = in[stride];
  in2 = in[stride * 2];
  in3 = in[stride * 3];
  in4 = in[stride * 4];
  in5 = in[stride * 5];
  in6 = in[stride * 6];
  in7 = in[stride * 7];
  tmp10 = (in0 + in4) * 46341;
  tmp11 = (in0 - in4) * 46341;
  tmp12 = in2 * 25080 - in6 * 60547;
  tmp13 = in6 * 25080 + in2 * 60547;
  tmp20 = tmp10 + tmp13;
  tmp21 = tmp11 + tmp12;
  tmp22 = tmp11 - tmp12;
  tmp23 = tmp10 - tmp13;
  tmp30 = ((((in3 + in5) * 46341) + (1L << (16 - 1 - 2))) >> (16 - 2));
  tmp31 = ((((in3 - in5) * 46341) + (1L << (16 - 1 - 2))) >> (16 - 2));
  tmp40 = ((in1) << 2) + tmp30;
  tmp41 = ((in7) << 2) + tmp31;
  tmp42 = ((in1) << 2) - tmp30;
  tmp43 = ((in7) << 2) - tmp31;
  tmp50 = tmp40 * 16069 + tmp41 * 3196;
  tmp51 = tmp40 * 3196 - tmp41 * 16069;
  tmp52 = tmp42 * 9102 + tmp43 * 13623;
  tmp53 = tmp42 * 13623 - tmp43 * 9102;
  in[0] = (((tmp20 + tmp50) + (1L << 16)) >> (16 + 1));
  in[stride] = (((tmp21 + tmp53) + (1L << 16)) >> (16 + 1));
  in[stride * 2] = (((tmp22 + tmp52) + (1L << 16)) >> (16 + 1));
  in[stride * 3] = (((tmp23 + tmp51) + (1L << 16)) >> (16 + 1));
  in[stride * 4] = (((tmp23 - tmp51) + (1L << 16)) >> (16 + 1));
  in[stride * 5] = (((tmp22 - tmp52) + (1L << 16)) >> (16 + 1));
  in[stride * 6] = (((tmp21 - tmp53) + (1L << 16)) >> (16 + 1));
  in[stride * 7] = (((tmp20 - tmp50) + (1L << 16)) >> (16 + 1));
  return 0;
}

j_rev_dct(short *data)
{
  int ivariable, j, k, id;
  for (ivariable = 0; ivariable < 8; ivariable++)
    fast_idct_8(data + ivariable * 8, 1);
  for (ivariable = 0; ivariable < 8; ivariable++)
    fast_idct_8(data + ivariable, 8);
  return 0;
}

// matriz de quantizacao
unsigned char qtbl_lum[] = {16, 11, 10, 16, 24, 40, 51, 61,
			    12, 12, 14, 19, 26, 58, 60, 55,
			    14, 13, 16, 24, 40, 57, 69, 56,
			    14, 17, 22, 29, 51, 87, 80, 62,
			    18, 22, 37, 56, 68, 109, 103, 77,
			    24, 35, 55, 64, 81, 104, 113, 92,
			    49, 64, 78, 87, 103, 121, 120, 101,
			    72, 92, 95, 98, 112, 100, 103, 99};
dquantz_lum(short *data)
{
  int ivariable;
  for (ivariable = 0; ivariable < 8 * 8; ivariable++)
    *(data + ivariable) = *(data + ivariable) * qtbl_lum[ivariable];
  return 0;
}

static short dct_data[240 * 160];

int jpeg_main()
{

  int ivariable, j, k, l, m;
  short *pdct;
  int prev;
  int value, npixels;
  int sum;

  lastlong = 0;
  bitsleft = 0;
  nextlong = huffbits;
  prev = 0;
  rvex_succeed("debug1\n");
  for (ivariable = 0; ivariable < 600; ivariable++)
    {
      huff_dc_dec(&value);
      dct_data[ivariable * 8 * 8] = value + prev;
      prev = dct_data[ivariable * 8 * 8];
    }

  for (ivariable = 0; ivariable < 600; ivariable++)
    huff_ac_dec(&dct_data[ivariable * 64]);

  pdct = &dct_data[0];

  for (ivariable = 0; ivariable < 600; ivariable++)
    dquantz_lum(pdct + ivariable * 64);

  pdct = &dct_data[0];

  for (ivariable = 0; ivariable < 600; ivariable++)
    j_rev_dct(pdct + ivariable * 64);

  npixels = 600 * 64;
  sum = 0;

  rvex_succeed("debug\n");
  for (ivariable = m = 0; ivariable < npixels; ivariable = ivariable + 8 * ncols)
    {
      for (j = 0; j < 64; j = j + 8)
	{
	  for (k = 0; k < 8 * ncols; k = k + 64)
	    {
	      for (l = 0; l < 8; l++)
		{
		  sum += dct_data[l + k + j + ivariable] + 64;
		}
	    }
	}
    }

  if (sum != 2598822)
    {
      rvex_fail("jpeg: fail\n");
    }
  else
    {
      rvex_succeed("jpeg: success\n");
      return 0;
    }
    rvex_fail("jpeg: fail\n");
  return 1;
}

/*Jpeg Benchmark code Area*/




int test1_main(){
  int i = 0;
  int y = 2;
  for ( i ; i<350; i++){
    i = i + y;
  }

  if(i == 351){
    rvex_succeed("Test 1 completed!\n");
    return 0;
  }else{
    rvex_fail("Test 1 failed!\n");
  }
  return 0;
}

test2_main(){
  int j = 0;
  int h = 2;
  for (j ; j<100; j++){
    j = j + h;
  }

  if(j == 102){
      rvex_succeed("Test 2 completed!\n");
      return 0;
  }else{
    rvex_fail("Test 2 failed!\n");
  }

  return 0;
}

//Main Area

int main (){

  if (CR_CID == 0) {
    // Context 0:
    CR_CRR = 0x0011;

    jpeg_main();


  } else {
    // Context 1:

    ucbqsort_main();
  }

}
