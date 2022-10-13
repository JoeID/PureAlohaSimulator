all: TD4_2.cpp TD4_2
	./TD4_2
	python3 TD4_2_plot.py
	python3 TD4_2_plot_success.py

TD4_2: TD4_2.cpp
	g++ -o TD4_2 TD4_2.cpp

clean:
	rm TD4_2 res.txt res_success.txt
