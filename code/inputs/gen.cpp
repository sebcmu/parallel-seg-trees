#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <array>
#include <fstream>
#include <ctime>

int main(int argc,char* argv[]){
    /* Seeding with the time allows randomization */
    srand(time(NULL));
    int array_size = -1;
    int num_ops = -1;
    std::string filename;
    // defaults
    int range = 10;
    float query_ratio = 0.5;
    std::string dir = "testinputs";
    int opt;
    while((opt = getopt(argc,argv,"n:o:q:d:f:")) != -1){
        switch (opt){
            case 'n':
                array_size = atoi(optarg);
                break;
            case 'o':
                num_ops = atoi(optarg);
                break;
            case 'q':
                query_ratio = atof(optarg);
                break;
            case 'd':
                dir = optarg;
                break;
            case 'f':
                filename = optarg;
                break;
            case 'r':
                /* This is the one-sided range around zero */
                /* For example, range=10 impies the random generated values for updates are between -10 and 10 */
                range = atoi(optarg);
                break;
            default:
                std::cerr << "Usage: " << argv[0] << "./gen -n <array_size> -o <num_ops> -r <value_range> -q <query_ratio> -d <output_directory> -f <output_filename>\n";
        }
    }

    // Check if required options are provided
    if (empty(filename) || array_size <= 0 || num_ops <= 0 || query_ratio <= 0 || query_ratio >= 1) {
        std::cerr << "Usage: " << argv[0] << "./gen -n <array_size> -o <num_ops> -q <query_ratio> -d <output_directory> -f <output_filename> (-n, -o, -f are required, -n -o must be < 0, -q must be between 0 and 1 inclusive)\n";;
    }

    /* Code to generate a new file */
    std::string output_path = dir + "/" + filename;
    std::ofstream fout(output_path);
    if (!fout) {
        std::cerr << "Error in opening output file on path " << output_path << "\n";
    }

    /* Maintain validation array to write validation part of test file */
    std::vector<int> validation_array(array_size);
    /* Maintain query answers to populate per query */
    std::vector<int> query_answers;
    std::vector<std::array<int,3>> ops(num_ops);
    int num_query = 0;
    
    for(int op_i = 0; op_i < num_ops; op_i++) {
        double randomness = static_cast<double>(rand()) / RAND_MAX;
        if (randomness < query_ratio){
            /* Update is 0, Query is 1 */
            /* Less than query_ratio implies this is a query, which is 1 in our formulation */
            ops[op_i][0] = 1;
            int random_index_1 = static_cast<int>((static_cast<double>(rand()) / RAND_MAX) * static_cast<double>(array_size));
            int random_index_2 = static_cast<int>((static_cast<double>(rand()) / RAND_MAX) * static_cast<double>(array_size));
            /* Want to create separate indices so that queries are well-formed */
            while (random_index_2 == random_index_1) {
                random_index_2 = static_cast<int>((static_cast<double>(rand()) / RAND_MAX) * static_cast<double>(array_size));
            }
            ops[op_i][1] = std::min(random_index_1,random_index_2);
            ops[op_i][2] = std::max(random_index_1,random_index_2);
            
            /* Perform the query and append result to query_answers */
            int query_answer = 0;
            for (int index = ops[op_i][1]; index < ops[op_i][2]; index++){
                query_answer += validation_array[index];
            }
            query_answers.push_back(query_answer);

            /* Update number of queries */
            num_query++;
        } else {
            ops[op_i][0] = 0;
            /* For now, pick indices at random from all array positions */
            /* Maybe change this later */
            int random_index = static_cast<int>((static_cast<double>(rand()) / RAND_MAX) * static_cast<double>(array_size));
            if (random_index == array_size){
                random_index = array_size - 1;
            }
            /* Static cast int rounds towards zero, this is fine */
            int random_value = static_cast<int>((static_cast<double>(rand()) / RAND_MAX) * static_cast<double>(range * 2) - static_cast<double>(range));
            ops[op_i][1] = random_index;
            ops[op_i][2] = random_value;

            /* Perform update on array */
            validation_array[random_index] = random_value;
        }
        
    }
    
    int num_update = num_ops - num_query;

    // write to file
    fout << array_size << "\n";
    fout << num_update << " " << num_query << "\n";
    for (int op_i = 0; op_i < num_ops; op_i++){
        if (ops[op_i][0] == 1) {
            fout << "q" << " " << ops[op_i][1] << " " << ops[op_i][2] << "\n";
        } else {
            fout << "u" << " " << ops[op_i][1] << " " << ops[op_i][2] << "\n";
        }
    }
    for (int query_i = 0; query_i < num_query; query_i++) {
        fout << query_answers[query_i] << "\n";
    }

    return 0;
}