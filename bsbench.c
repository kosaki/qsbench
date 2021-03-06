/*
 * Copyright (C) 2002 Lorenzo Allegrucci (lenstra@tiscalinet.it)
 * Licensed under the GPL
 */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_PROCS	1024

int verbose = 0;

void quick_sort(int a[], int l, int r);
void do_qsort(int n, int s, int sleep_time);
void start_procs(int n, int p, int s, int sleep_time);
void usage(void);

/**
 * quick_sort - Sort in the range [l, r]
 */
void quick_sort(int a[], int l, int r)
{
	int i, j, p, tmp;
	int m, min, max;

	i = l;
	j = r;
	m = (l + r) >> 1;

	if (a[m] >= a[l]) {
		max = a[m];
		min = a[l];
	} else {
		max = a[l];
		min = a[m];
	}

	if (a[r] >= max)
		p = max;
	else {
		if (a[r] >= min)
			p = a[r];
		else
			p = min;
	}

	do {
		while (a[i] < p)
			i++;
		while (p < a[j])
			j--;
		if (i <= j) {
			tmp = a[i];
			a[i] = a[j];
			a[j] = tmp;
			i++;
			j--;
		}
	} while (i <= j);

	if (l < j)
		quick_sort(a, l, j);
	if (i < r)
		quick_sort(a, i, r);
}


void do_qsort(int n, int s, int sleep_time)
{
	int * a, i, errors = 0;

	if ((a = malloc(sizeof(int) * n)) == NULL) {
		perror("malloc");
		exit(1);
	}

	srand(s);
	if (verbose)
		printf("seed = %d\n", s);

	for (i = 0; i < n; i++)
		a[i] = rand();

	if (verbose) {
		printf("sorting... ");
		fflush(stdout);
	}
	quick_sort(a, 0, n - 1);
	if (verbose)
		printf("done.\n");

	if (verbose) {
		printf("verify... ");
		fflush(stdout);
	}
	for (i = 0; i < n - 1; i++)
		if (a[i] > a[i + 1]) {
			errors++;
			fprintf(stderr, "ERROR: i = %d\n", i);
		}
	if (verbose)
		printf("done.\n");

	if (errors)
		fprintf(stderr, " *** WARNING ***  %d errors.\n", errors);

	sleep(sleep_time);
	free(a);
	exit(0);
}


void start_procs(int n, int p, int s, int sleep_time)
{
	int i, pid[MAX_PROCS];
	int status;

	if (p > MAX_PROCS)
		p = MAX_PROCS;

	for (i = 0; i < p; i++) {
		pid[i] = fork();
		if (pid[i] == 0) {
			do_qsort(n, s, sleep_time);
		} else {
			if (pid[i] < 0)
				perror("fork");
			sleep(1);
		}
	}

	for (i = 0; i < p; i++)
		waitpid(pid[i], &status, 0);
}

void usage(void)
{
	fprintf(stderr, "Usage: bsbench [-h] [-m MB] [-p nr_procs]"
			" [-s seed] [-v] [-z sleep_time]\n");
	exit(1);
}


int main(int argc, char * argv[])
{
	char *m = "10", *p = "1", *s = "1", *st = "0";
	int nr_elems, nr_procs, seed, sleep_time;
	int c;

	if (argc == 1)
		usage();

	while (1) {
		c = getopt(argc, argv, "hm:p:s:vz:V");
		if (c == -1)
			break;

		switch (c) {
		case 'h':
			usage();
		case 'm':
			m = optarg;
			break;
		case 'p':
			p = optarg;
			break;
		case 's':
			s = optarg;
			break;
		case 'v':
			verbose = 1;
			break;
		case 'z':
			st = optarg;
			break;
		case 'V':
			printf("Version 1.0.0\n");
			return 1;
		case '?':
			return 1;
		}
	}

	nr_elems = (atoi(m) * 1048576) / 4;
	nr_procs = atoi(p);
	seed = atoi(s);
	sleep_time = atoi(st);
	start_procs(nr_elems, nr_procs, seed, sleep_time);

	return 0;
}
