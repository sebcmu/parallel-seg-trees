#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <functional>
using IntCombine = std::function<int(int,int)>;

int main(int argc,char* argv[]){
    /* Seeding with the time allows randomization */
    srand(time(NULL));
    int array_size = -1;
    int num_ops = -1;
    std::string filename;
    
    /* Defaults */
    int chunk_size = 1;
    /* The advantages of CGL and FGL will only be felt when there are multiple queries in a row and multiple updates in a row */
    /* For queries, they must wait for previous updates. They are going to take reading locks, so overlapping queries can provide an advantage. */
    /* For updates, they can overlap in a FGL structure since they don't rely on each other, so overlapping updates can provide an advantage. */
    int range = 10;
    /* Range defines the one-sided width around 0 for the generated values */
    /* For example, range=10 impies the random generated values for updates are between -10 and 10 */
    /* We generate around 0 so that the sum doesn't get too large or small */
    float query_ratio = 0.5;
    std::string dir = "testinputs";
    std::string combine_fn_str = "sum";
    IntCombine combine_fn = [](int a, int b){return a+b;};
    
    int opt;
    while((opt = getopt(argc,argv,"n:o:q:d:f:c:r:p:h")) != -1){
        switch (opt){
            /* Logistical parameters */
            case 'd': dir = optarg;                             break;
            case 'f': filename = optarg;                        break;
            /* Details */
            case 'n': array_size = atoi(optarg);                break;
            case 'o': num_ops = atoi(optarg);                   break;
            case 'q': query_ratio = atof(optarg);               break;
            case 'c': chunk_size = atoi(optarg);                break;
            case 'r': range = atoi(optarg);                     break;
            /* Special uses */
            case 'p': combine_fn_str = optarg;                  break;
            /* Help */
            case 'h':
                std::cout << "./gen\n";
                std::cout << std::left;
                std::cout << std::setw(35) << "  -d <output_directory>"     << "Defines the output directory\n";
                std::cout << std::setw(35) << "  -f <output_filename>"    << "Defines the output file name\n";
                std::cout << std::setw(35) << "  -n <array_size>"         << "Size of the array\n";
                std::cout << std::setw(35) << "  -o <num_ops>"            << "Number of operations (update, query) to apply to the array\n";
                std::cout << std::setw(35) << "  -q <query_ratio>"        << "Proportion of operations that are queries\n";
                std::cout << std::setw(35) << "  -c <chunk_size>"         << "Size of operation chunks (must divide the number of operations) \n";
                std::cout << std::setw(35) << "  -r <range>"              << "Generates updates in (-range,range)\n";
                std::cout << std::setw(35) << "  -p <combine_fn_string>"  << "String of the combining function (Options: sum, max, min)\n";
                return 1;
            default:
                std::cerr << "Usage Help: ./gen -h\n";
                return 1;
        }
    }

    /* Check if required options are provided */
    if (empty(filename) || array_size <= 0 || num_ops <= 0 || query_ratio <= 0 || query_ratio >= 1 || num_ops % chunk_size != 0) {
        std::cerr << "Usage Help: ./gen -h\n";;
        return 1;
    }

    /* Set Combine Function */

    if(combine_fn_str == "sum"){
        combine_fn = [](int a, int b){return a+b;};
    }
    else if(combine_fn_str == "max"){
        combine_fn = [](int a, int b){return std::max(a,b);};
    }
    else if(combine_fn_str == "min"){
        combine_fn = [](int a, int b){return std::min(a,b);};
    }
    else{
        /* Default Case If No Match on Combine Function */
        combine_fn = [](int a, int b){return a+b;};
        std::cout << "Combine function input: " << combine_fn_str <<  " did not match an available option. Using sum.";
    }

    /* Code to generate a new file */
    /* Add information about the parameters used to generate the test case */
    std::stringstream q_float_stream;
    q_float_stream << std::fixed << std::setprecision(2) << query_ratio;
    std::string query_rep = q_float_stream.str();
    std::string altered_filename = filename + "_n_" + std::to_string(array_size) + "_o_" + std::to_string(num_ops) + "_c_" + std::to_string(chunk_size) + "_r_" + std::to_string(range) + "_q_" + query_rep + "_p_" + combine_fn_str;
    std::string output_path = dir + "/" + altered_filename + ".txt";
    std::ofstream fout(output_path);
    if (!fout) {
        std::cerr << "Error in opening output file on path " << output_path << "\n";
        return 1;
    }

    /* Maintain validation array to write validation part of test file */
    std::vector<int> validation_array(array_size);
    /* Maintain query answers to populate per query */
    std::vector<int> query_answers;
    std::vector<std::array<int,3>> ops(num_ops);
    int num_query = 0;
    
    for(int op_chunk = 0; op_chunk < num_ops; op_chunk += chunk_size) {
        double randomness = static_cast<double>(rand()) / RAND_MAX;
        if (randomness < query_ratio){
            /* Update is 0, Query is 1 */
            /* Less than query_ratio implies this is a query, which is 1 in our formulation */
            for (int op_i = op_chunk; op_i < op_chunk + chunk_size; op_i++){
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
                    query_answer = combine_fn(query_answer,validation_array[index]);
                }
                query_answers.push_back(query_answer);

                /* Update number of queries */
                num_query++;
            }
        } else {
            for (int op_i = op_chunk; op_i < op_chunk + chunk_size; op_i++){
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
                validation_array[random_index] = combine_fn(validation_array[random_index],random_value);
            }
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