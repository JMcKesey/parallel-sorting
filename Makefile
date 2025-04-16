build:
	gcc merge-sort-sequential.c -o sequential -O3
	gcc merge-sort-openmp.c -o openmp -fopenmp -O3
	gcc merge-sort-pthread.c -o pthread -lpthread -O3
	gcc radix-sort-openmp.c -o radix-openmp -fopenmp -O3
	gcc radix-sort-seq.c -o radix-seq -O3
	echo "Build completed!"

clean:
	rm -f sequential
	rm -f openmp
	rm -f pthread
	rm -f radix-openmp
	rm -f radix-seq
	echo ".o files deleted!"