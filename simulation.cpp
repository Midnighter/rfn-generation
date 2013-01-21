/*******************************************************************************
 * filename:    simulation.cpp
 * description: Main program parses options and instantiates classes
 * author:      Moritz Beber
 * created:     2010-04-17
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// std
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cassert>
#include <typeinfo>
#include <sys/types.h>
#include <unistd.h>


// boost
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/algorithm/string/predicate.hpp>

// project
#include "rfn_config.hpp"
#include "common_definitions.hpp"
#include "C_ParameterManager.hpp"
#include "C_RobustParameterManager.hpp"
#include "C_NoiseRobustParameterManager.hpp"
#include "C_NetworkBuilder.hpp"
#include "C_FlowDistributionNetwork.hpp"
#include "C_Evolution.hpp"


/*******************************************************************************
 * Function Declarations
 ******************************************************************************/


std::vector<std::string>* parse_arguments(int argc, char** argv);
void verify_parameters(rfn::ParameterManager* const parameters);
bool verify_name(std::string dup_name, bfsys::path dir);


/*******************************************************************************
 * Function Definitions
 ******************************************************************************/


int main(int argc, char** argv) {
    using std::cout;
    using std::cerr;
    using std::endl;
    // turn off GSL error handler
//    gsl_error_handler_t* const old_handler = gsl_set_error_handler_off();
    gsl_set_error_handler_off();
    // network builder, singleton
    rfn::NetworkBuilder* builder = rfn::NetworkBuilder::instance();
    // vector of networks to simulate from parameters
    std::vector<std::string>* const names =
        parse_arguments(argc, argv);
    // get parameter singleton instance
    rfn::ParameterManager* const parameters = rfn::ParameterManager::instance();
    // define evolution type from parameters
    rfn::Evolution* const evolution =
        rfn::Evolution::instance(parameters->network_type());
    // choose a seed if not given
    if (parameters->seed() == 0) {
        parameters->seed(std::time(NULL) + getpid());
    }
    rfn::RandGen rng_state(parameters->rng());
    bool conn_not_given = true;
    if (parameters->connectivity() != 0.0) {
        conn_not_given = false;
    }
    // for each network perform evolution
    rfn::FlowDistributionNetwork* network = NULL;
    std::string dup_name;
    bfsys::path dir;
    bfsys::path tmp;
    bfsys::ofstream output;
    bool name_exists = false;
    for (std::vector<std::string>::iterator name_it =
        names->begin(); name_it != names->end(); ++name_it)
    {
        parameters->rng() = rng_state;
        // choose connectivity if not given
        if (conn_not_given) {
            std::srand(parameters->seed());
            parameters->connectivity((double)std::rand() / (double)RAND_MAX);
        }
        dir = *name_it;
        // verify network name
        if (bfsys::exists(dir.parent_path())) {
            dup_name = dir.filename();
            try {
                name_exists = verify_name(dup_name, dir.parent_path());
            }
            catch (bfsys::basic_filesystem_error<bfsys::path>& e) {
                name_exists = false;
                dup_name.append("_duplicate");
            }
            while (name_exists) {
                dup_name.append("_duplicate");
                try {
                    name_exists = verify_name(dup_name, dir.parent_path());
                }
                catch (bfsys::basic_filesystem_error<bfsys::path>& e) {
                    name_exists = false;
                }
            }
        }
        else if (dir.parent_path() == "") {
            dup_name = dir.filename();
            dir = ".";
            try {
                name_exists = verify_name(dup_name, dir);
            }
            catch (bfsys::basic_filesystem_error<bfsys::path>& e) {
                name_exists = false;
                dup_name.append("_duplicate");
            }
            while (name_exists) {
                dup_name.append("_duplicate");
                try {
                    name_exists = verify_name(dup_name, dir);
                }
                catch (bfsys::basic_filesystem_error<bfsys::path>& e) {
                    name_exists = false;
                }
            }
        }
        else {
            cerr << "Directory does not exist, skipping." << endl;
            continue;
        }
        network = builder->build(parameters, bfsys::path(dir.parent_path() /
            dup_name).string());
        // evolution
        evolution->run(network);
        delete network;
        network = NULL;
        // store parameters
        tmp = dir.parent_path() / "parameters.dat";
        if (!bfsys::exists(tmp)) {
            output.open(tmp, std::ios::binary);
            parameters->write_to_binary_stream(output);
            output.close();
        }
    }
    // clean-up
    builder = NULL; // singleton destroys itself
    names->clear();
    delete names;
    return (EXIT_SUCCESS);
}

/*
 *
 */
std::vector<std::string>*
parse_arguments(int argc, char** argv)
{
    using std::cout;
    using std::endl;
    using std::cerr;
    // generic command line options
    bpo::options_description generic("Generic options");
    generic.add_options()
        ("help,h", "print help message")
        ("version,v", "print version information")
        ("config-file", bpo::value<std::string>()->default_value
            ("simulation.cfg"),
            "specify an alternative configuration file (path)")
    ;
    // required command line or configuration file options
    bpo::options_description required("Required options");
    required.add_options()
        ("network-name", bpo::value<std::vector<std::string> >()->composing(),
            "name the network that is evolved (string)\n"
            "specify multiple times to simulate more networks"
            " or simply add the names after all other options")
        ("flow,f", "perform flow evolution")
        ("link-robust,l", "perform link robust evolution")
        ("node-robust,n", "perform node robust evolution")
        ("noise-robust,w", bpo::value<double>(),
            "perform noise robust evolution with a given intensity (decimal)\n"
            "\nnote: f, l, n, w are all mutually exclusive")
    ;
    // additional options
    bpo::options_description additional("Additional options");
    additional.add_options()
        ("input-nodes,i", bpo::value<short unsigned>()->default_value(8),
            "number of input layer nodes (integer)")
        ("middle-nodes,m", bpo::value<short unsigned>()->default_value(20),
            "number of middle layer nodes (integer)")
        ("output-nodes,o", bpo::value<short unsigned>()->default_value(8),
            "number of output layer nodes (integer)")
        ("activated-nodes,k", bpo::value<short unsigned>()->default_value(4),
            "number of activated output layer nodes (integer)")
        ("flow-threshold,t", bpo::value<double>()->default_value(0.007),
            "threshold for the flow error (decimal)")
        ("flow-factor,s", bpo::value<double>()->default_value(0.0001),
            "factor in acceptance of mutation due to annealing (decimal)")
        ("evolution-time,e", bpo::value<unsigned long>()->default_value(100000),
            "maximum number of iteration steps performed in evolution (integer)")
        ("snapshots", bpo::value<short unsigned>(),
            "number of records to keep of the evolution (integer)")
        ("seed,g", bpo::value<unsigned>(),
            "provide a seed for random number generation (integer)")
        ("network-init", bpo::value<std::string>()->default_value("random"),
            "how to initialise the network ('random' | 'fixed')")
        ("pattern-init", bpo::value<std::string>()->default_value("random"),
            "how to initialise the ideal pattern ('random' | 'complexity'"
            " | high-complexity | low-complexity | equal-complexity | "
            "equal-spread)")
        ("mutation", bpo::value<std::string>()->default_value("path"),
            "how to mutate the network ('path' | 'link')")
        ("connectivity,c", bpo::value<double>(),
            "connectivity of the initial network (decimal between 0 and 1)")
        ("add-to", bpo::value<double>()->default_value(0.5),
            "probability of adding a mutation to the network (decimal between"
            " 0 and 1)")
        ("robust-threshold", bpo::value<double>()->default_value(0.007),
            "threshold for the error in robustness computations (decimal)")
        ("robust-factor", bpo::value<double>()->default_value(0.0001),
            "factor in acceptance of mutation due to annealing in robustness"
            " (decimal)")
        ("min-robustness", bpo::value<double>()->default_value(1.0),
            "minimum robustness the network should evolve to (decimal between"
            " 0 and 1)")
        ("lower-bound", bpo::value<double>(),
            "lower bound for ideal pattern complexity")
        ("upper-bound", bpo::value<double>(),
            "upper bound for ideal pattern complexity")
    ;
    // positional options
    bpo::positional_options_description positional;
    positional.add("network-name", -1);
    // separate groups into
    // command line options
    bpo::options_description cmd_line;
    cmd_line.add(generic).add(required).add(additional);
    // configuration file options
    bpo::options_description file;
    file.add(required).add(additional);
    // arguments map
    bpo::variables_map vm;
    // parse the command line options
    try {
        bpo::store(bpo::command_line_parser(argc, argv).
            options(cmd_line).positional(positional).run(), vm);
        bpo::notify(vm);
        bfsys::path config_file(vm["config-file"].as<std::string>());
        if (bfsys::exists(config_file)) {
            // parse configuration file
            bfsys::ifstream input(config_file);
            bpo::store(bpo::parse_config_file(input, file), vm);
            bpo::notify(vm);
        }
    }
    catch (std::exception& e) {
        cerr << e.what() << endl;
        std::exit(EXIT_FAILURE);
    }
    if (vm.count("help")) {
        cout << "Usage:" << endl;
        cout << argv[0] << " <required options> [additional options]"
            " <name1> [name2] [...]" << endl;
        cout <<  cmd_line << endl;
        std::exit(EXIT_SUCCESS);
    }
    if (vm.count("version")) {
        cout << RFN5_VERSION_MAJOR << "." << RFN5_VERSION_MINOR << "."
            << RFN5_VERSION_PATCH << endl;
        std::exit(EXIT_SUCCESS);
    }
    // make sure at least one network name has been supplied
    if (vm.count("network-name") == 0) {
        cerr << "Error: No network name supplied!" << endl;
        std::exit(EXIT_FAILURE);
    }
    // network type
    rfn::ParameterManager* parameters = NULL;
    if (vm.count("flow")) {
        parameters = rfn::ParameterManager::instance(rfn::flow_evolved);
    }
    else if (vm.count("link-robust")) {
        parameters = rfn::ParameterManager::instance(rfn::link_robust);
    }
    else if (vm.count("node-robust")) {
        parameters = rfn::ParameterManager::instance(rfn::node_robust);
    }
    else if (vm.count("noise-robust")) {
        parameters = rfn::ParameterManager::instance(rfn::noise_robust);
    }
    else {
        cerr << "No evolution type specified. Aborting." << endl;
        std::exit(EXIT_FAILURE);
    }
    CHECK_CRITICAL_POINTER(parameters);
    // network initialisation scheme
    if (vm["network-init"].as<std::string>() == "random") {
        parameters->network_init(rfn::random);
    }
    else if (vm["network-init"].as<std::string>() == "fixed") {
        parameters->network_init(rfn::list);
    }
    else {
        cerr << "No known network initialisation scheme specified. Aborting."
            << endl;
        std::exit(EXIT_FAILURE);
    }
    // pattern initialisation scheme
    if (vm["pattern-init"].as<std::string>() == "random") {
        parameters->pattern_init(rfn::random_k);
    }
    else if (vm["pattern-init"].as<std::string>() == "complexity") {
        parameters->pattern_init(rfn::complexity);
    }
    else if (vm["pattern-init"].as<std::string>() == "high-complexity") {
        parameters->pattern_init(rfn::high_complexity);
    }
    else if (vm["pattern-init"].as<std::string>() == "low-complexity") {
        parameters->pattern_init(rfn::low_complexity);
    }
    else if (vm["pattern-init"].as<std::string>() == "equal-complexity") {
        parameters->pattern_init(rfn::equal_complexity);
    }
    else if (vm["pattern-init"].as<std::string>() == "equal-spread") {
        parameters->pattern_init(rfn::equal_spread_complexity);
    }
    else {
        cerr << "No known ideal pattern initialisation scheme specified."
            " Aborting." << endl;
        std::exit(EXIT_FAILURE);
    }
    // mutation type
    if (vm["mutation"].as<std::string>() == "path") {
        parameters->mutation_type(rfn::path_mutation);
    }
    else if (vm["mutation"].as<std::string>() == "link") {
        parameters->mutation_type(rfn::link_mutation);
    }
    else {
        cerr << "No known mutation scheme specified. Aborting." << endl;
        std::exit(EXIT_FAILURE);
    }
    // set other parameters
    parameters->nodes_in(vm["input-nodes"].as<short unsigned>());
    parameters->nodes_middle(vm["middle-nodes"].as<short unsigned>());
    parameters->nodes_out(vm["output-nodes"].as<short unsigned>());
    parameters->activated_k(vm["activated-nodes"].as<short unsigned>());
    parameters->error_threshold(vm["flow-threshold"].as<double>());
    parameters->annealing_factor(vm["flow-factor"].as<double>());
    parameters->max_time(vm["evolution-time"].as<unsigned long>());
    if (vm.count("snapshots")) {
        parameters->snapshots(vm["snapshots"].as<short unsigned>());
    }
    if (vm.count("seed")) {
        parameters->seed(vm["seed"].as<unsigned>());
    }
    if (vm.count("connectivity")) {
        parameters->connectivity(vm["connectivity"].as<double>());
    }
    if (vm.count("lower-bound") &&
            parameters->pattern_init() == rfn::complexity)
    {
        parameters->lower_bound(vm["lower-bound"].as<double>());
    }
    if (vm.count("upper-bound") &&
            parameters->pattern_init() == rfn::complexity)
    {
        parameters->upper_bound(vm["upper-bound"].as<double>());
    }
    parameters->add_mutation(vm["add-to"].as<double>());
    // set parameters for robust networks
    if (typeid(*parameters) == typeid(rfn::RobustParameterManager)) {
        rfn::RobustParameterManager* robust_parameters = static_cast
            <rfn::RobustParameterManager*>(parameters);
        robust_parameters->min_robustness(vm["min-robustness"].as<double>());
        robust_parameters->robust_threshold(vm["robust-threshold"].as<double>());
        robust_parameters->robust_factor(vm["robust-factor"].as<double>());
    }
    // set parameters for noise robust networks
    if (typeid(*parameters) == typeid(rfn::NoiseRobustParameterManager)) {
        rfn::NoiseRobustParameterManager* robust_parameters = static_cast
            <rfn::NoiseRobustParameterManager*>(parameters);
        robust_parameters->min_robustness(vm["min-robustness"].as<double>());
        robust_parameters->robust_threshold(vm["robust-threshold"].as<double>());
        robust_parameters->robust_factor(vm["robust-factor"].as<double>());
        robust_parameters->noise_level(vm["noise-robust"].as<double>());
    }
    // test sensibility of parameters provided
    verify_parameters(parameters);
    // build networks
    std::vector<std::string>* names = new std::vector<std::string>(vm["network-name"].
        as<std::vector<std::string> >());
    return names;
}

void
verify_parameters(rfn::ParameterManager* const parameters)
{
    using std::cerr;
    using std::endl;
    // test expected parameter ranges
    assert(parameters->nodes_in() > 0);
    assert(parameters->nodes_middle() > 0);
    assert(parameters->nodes_out() > 0);
    if (parameters->nodes_in() >= parameters->nodes_middle()
        || parameters->nodes_out() >= parameters->nodes_middle())
    {
        cerr << "Warning: It is advisable that the network middle layer contains"
            " the most nodes." << endl;
    }
    assert(parameters->activated_k() > 0
        && parameters->activated_k() <= parameters->nodes_out());
    assert(parameters->connectivity() >= 0.0
        && parameters->connectivity() <= 1.0);
    assert(parameters->add_mutation() >= 0.0
        && parameters->add_mutation() <= 1.0);
    // test expected parameter ranges for robust networks
    if (typeid(*parameters) == typeid(rfn::RobustParameterManager)) {
        rfn::RobustParameterManager* robust_parameters = static_cast
            <rfn::RobustParameterManager*>(parameters);
        assert(robust_parameters->min_robustness() >= 0.0
            && robust_parameters->min_robustness() <= 1.0);
    }
    // test expected parameter ranges for noise robust networks
    if (typeid(*parameters) == typeid(rfn::NoiseRobustParameterManager)) {
        rfn::NoiseRobustParameterManager* robust_parameters = static_cast
            <rfn::NoiseRobustParameterManager*>(parameters);
        if (robust_parameters->noise_level() == 0.0) {
            cerr << "Warning: Noise robust networks with zero noise intensity"
                " reduce to node robust networks." << endl;
        }
        if (robust_parameters->noise_level() >= 1.0) {
            cerr << "Warning: Noise robust networks with such high noise"
                " intensity are unlikely to evolve properly." << endl;
        }
    }
}

bool
verify_name(std::string dup_name, bfsys::path dir)
{
    bfsys::directory_iterator end_it;
    for (bfsys::directory_iterator itr(dir); itr != end_it; ++itr) {
        if (!bfsys::is_directory(itr->path())) {
            if (boost::starts_with(itr->path().filename(), dup_name)) {
                return true;
            }
        }
    }
    return false;
}
