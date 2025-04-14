build:
	gcc merge-sort-sequential.c -o sequential -O3
	gcc merge-sort-openmp.c -o openmp -fopenmp -O3
	gcc merge-sort-pthread.c -o pthread -lpthread -O3
	echo "Build completed!"

clean:
	rm -f sequential
	rm -f openmp
	rm -f pthread
	echo ".o files deleted!"