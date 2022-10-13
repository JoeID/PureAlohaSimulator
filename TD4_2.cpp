#include <iostream>
#include <cmath>
#include <vector>

#define EPS 0.0000001 // epsilon for float comparisons

using namespace std;

class RandomGenerator {
    private:
        double lambda;
    public:
        RandomGenerator(double x)
        {
            srand(time(0)); // initializes the seed
            this->lambda = x;
        }

        void set_lambda(double new_lambda) { this->lambda = new_lambda; }

        double getExpRandom()
        // random number following exponential distribution
        {
            double r = ((double)rand()/(double)RAND_MAX); // random btw 0 and 1
            return - log(1 - r)/this->lambda;
        }

        // Generates randomly N numbers and saves them in a textfile
        void testRandom(int N)
        {
            FILE *save = fopen("res.txt", "w+");
            fprintf(save, "%d\n%f\n", N, this->lambda);
            for(int i = 0; i < N; i++)
                fprintf(save, "%f\n", this->getExpRandom());
            fclose(save);
        }
};

typedef enum{
    RECEIVING_ONE,
    RECEIVING_SEVERAL,
    FREE,
} c_state; // channel states

class Channel {
    private:
        c_state state;
        double end_of_occupation; // end of last arrived packet
        double time; // actual time
        int successes; // number of packets successfully received
        int arrivals; // number of packets that arrived to the channel
        double packet_duration;

    public:
        Channel(double d)
        { // initially, the channel is unoccupied
            this->end_of_occupation = 0;
            this->time = 0;
            this->successes = 0;
            this->packet_duration = d;
            this->state = FREE;
        }

        double get_time() { return this->time; }

        double get_success_rate()
        { // percentage of time spent transmitting
            return ((double) this->successes * this->packet_duration) / (this->time);
        }

        void send_packet(double start_time)
        { 
            this->time = start_time;

            if(this->end_of_occupation < this->time + EPS){
                if(this->state == RECEIVING_ONE)
                    this->successes = this->successes + 1; // the last packet arrived has been successfully transmitted
                this->state = FREE; // channel becomes free at time start_time
            }
            // update state
            if(this->state == FREE){
                this->state = RECEIVING_ONE;
            } else if(this->state == RECEIVING_ONE){
                this->state = RECEIVING_SEVERAL;
            }
            this->end_of_occupation = this->time + this->packet_duration;
        }

        void end_simulation()
        { // checks if last packet has been successfully transmitted
            this->time = this->end_of_occupation;
            if(this->state == RECEIVING_ONE)
                this->successes = this->successes + 1;
        }
};

class System {
    public:
        double simulate(int n_arrivals, double lambda, double packet_duration)
        {
            RandomGenerator r_gen(lambda / packet_duration);
            Channel channel = Channel(packet_duration);

            for(int i = 0; i < n_arrivals; i++)
                channel.send_packet(channel.get_time() + r_gen.getExpRandom());
            channel.end_simulation();
            
            return channel.get_success_rate();
        }
};

int main(){
    int N = 1000 * 1000, n_arrivals = 100 * 1000, Nsim = 50;
    double G = 2, Gmin = 0.001, Gmax = 5, packet_duration = 100;
    RandomGenerator r_gen = RandomGenerator(G);
    System sys = System();

    // tests the random generator

    r_gen.testRandom(N);

    // tests the Pure Aloha protocol
    
    FILE *save = fopen("res_success.txt", "w+");
    fprintf(save, "%d %d\n", n_arrivals, Nsim); // Nsim simulations
    for(G = Gmin; G <= Gmax + EPS; G += G / Nsim){
        printf("G=%f\n", G);
        fprintf(save, "%f %f\n", G, sys.simulate(n_arrivals, G, packet_duration));
    }
    fclose(save);

    return 0;
}