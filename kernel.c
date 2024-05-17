#include <math.h> // log

void kernel (unsigned n, float a[n][n], float b[n][n], float x) {
  unsigned i ,j ,k;
  //  const float log_x = log(x);

  for (i=0; i<n ; i++)
    for (j=0; j<n ; j++)
      for (k=0; k<n; k++)
	a[i][j] += log(x) * b[j][k];
}
