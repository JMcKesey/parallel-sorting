build:
	gcc merge-sort-sequential.c -o sequential -O3
	gcc merge-sort-openmp.c -o parallel -fopenmp -O3
	gcc qsort-control.c -o control -O3
	"Build completed!"

clean:
	rm -f sequential
	rm -f parallel
	rm -f control
	".o files deleted!"