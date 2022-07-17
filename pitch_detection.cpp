#include <math.h>
#include <armadillo>
#include <map>
#include <iterator>

using namespace arma;
using namespace std;

/**
 * @brief Find the first 5 peak frequencies from X
 * 
 * @param X Input window of frequencies
 * @param locs Vector to store the indexes of first 5 peak frequencies
 */
void findpeaks (fvec& X, ivec& locs)
{
    locs = {-1, -1, -1, -1, -1};
    multimap<float, int, greater<float> > locs_map;
    multimap<float, int>::iterator it;
    int counter = 0;
    int j;
    for (j=1; j<(X.size()-1); j++) { 
        if ( X(j-1)<X(j) && X(j)>X(j+1) ) {
            locs_map.insert(make_pair(X(j), j));
        }
    }

    // // begin() returns to the first value of map     // This lines can be removed
    // for (it = locs_map.begin(); it != locs_map.end(); it++)
    //     cout << "(" << (*it).first << ", " << (*it).second
    //          << ")" << endl;
    // cout << "map size: " << locs_map.size() << endl;

    if (locs_map.size() < 5) {
        for (it = locs_map.begin(); it != locs_map.end(); counter++, it++) {
            locs(counter) = (*it).second;
        }
    } else {
        for (it = locs_map.begin(); counter < 5; counter++, it++) {
            locs(counter) = (*it).second;
        }
    }
  
    return;
}


/**
 * @brief Get the pitch of an input vector of samples
 * 
 * @param X Input window of samples
 * @return int Formant frequency
 */
float get_Pitch (fvec& X)
{
    fvec Y;
    vec f;
    ivec locs(5);
    vec pf(5);
    vec tff;
    double p;
    double ddf;
    float ff;
    int i;

    // for (i=0; i<10; i++) {
    //     cout << X(i) << endl;
    // } cout << endl;

    Y = real( fft( X, 1030 ) );
    Y = abs(Y);
    // cout << "Y lenght:  " << Y.size() << endl;
    // for (i=0; i<10; i++) {
    //     cout << Y(i) << endl;
    // } cout << endl;


    f = linspace<vec>(0, 1023, 1024);
    f /= 1024;

    // // We only need the lower half  
    Y = Y( span( 0, (1024/2)-1 ) );
    Y /= max(Y); // normalize


    // // Test print
    // cout << "Lenght of X:  " << X.size() << endl;
    // cout << "Lenght of Y:  " << Y.size() << endl;
    // cout << "Lenght of f:  " << f.size() << endl;
    // for (i=0; i<10; i++) {
    //     cout << Y(i) << endl;
    // } cout << endl;
    // cout << "Max element: " << max( Y ) << endl;
    // cout << "Max element index: " << index_max( Y ) << endl;



    // find first 5 peak frequencies indexes
    findpeaks(Y, locs);
    // cout << "locs indexes = {" << locs(0) << ", " << locs(1) << ", " << locs(2) \
    //     << ", " << locs(3) << ", " << locs(4) << "}" << endl;

    pf = {f(locs(0)), f(locs(1)), f(locs(2)), f(locs(3)), f(locs(4))};
    pf = sort(pf);

    // for (i=0; i<locs.size(); i++) {
    //     cout << "pf_" << i << ": " << pf(i) << endl;
    // }

    
    p = 10*log10(mean(square(X)));
    // cout << "Power = " << p << "dB" << endl;

    tff = pf / pf(0);
    // for (i=0; i<tff.size(); i++) {
    //     cout << "tff_" << i << ": " << tff(i) << endl;
    // }

    tff = tff - linspace<vec>(1, 5, 5);
    // for (i=0; i<tff.size(); i++) {
    //     cout << "tff_" << i << ": " << tff(i) << endl;
    // }

    tff = tff(span(0, 1));
    // cout << "tff size: " <<tff.size() << endl;
    // for (i=0; i<tff.size(); i++) {
    //     cout << "tff_" << i << ": " << tff(i) << endl;
    // }

    ff = pf(0);
    ddf = max(abs(tff));

    if (ddf>0.5 || ff>0.125 || p<-30) {
        // cout << "condition!" << endl;
        ff = 0;
    }

    return ff;
}



int main (void)
{
    float ff;

    // std::ifstream is("samples_for_get_Pitch.txt");
    std::ifstream is("sample_window_22.txt");
    std::istream_iterator<double> start(is), end;
    std::vector<double> numbers(start, end);

    // std::cout << "Read " << numbers.size() << " numbers" << std::endl;
    // // print the numbers to stdout
    // std::cout << "numbers read in:\n";
    // std::copy(numbers.begin(), numbers.end(), 
    //             std::ostream_iterator<double>(std::cout, " "));
    // std::cout << std::endl << std::endl;

    std::vector<float> float_numbers (numbers.begin(), numbers.end());
    fvec X = float_numbers;

    ff = get_Pitch(X);

    cout << "\nFormant: " << ff << endl;
    return 0;
}







// // Use this main only to test the "findpeaks" function and the Formant frequency returned
// // You have to add 1 to the indexes returned from "findpeaks" to match 
// // the ones obtained from Matlab, Matlab indexes start at "1", C/C++ at "0"
// int main (void)
// {

//     std::ifstream is("samples_for_findpeaks.txt");
//     std::istream_iterator<float> start(is), end;
//     std::vector<float> numbers(start, end);

//     // std::cout << "Read " << numbers.size() << " numbers" << std::endl;
//     // // print the numbers to stdout
//     // std::cout << "numbers read in:\n";
//     // std::copy(numbers.begin(), numbers.end(), 
//     //             std::ostream_iterator<float>(std::cout, " "));
//     // std::cout << std::endl << std::endl;


//     fvec Y = numbers;
//     // fvec Y = {0.5, 0.42, 0.2, 0.22, 0.21, 0.27, 0.34, 0.1, 0.003, 0.75, 0.69, 0.45};
//     // fvec Y = {0.5, 0.42, 0.2, 0.22, 0.21, 0.27, 0.34, 0.1, 0.003, 0.75, 0.69, \
//     //         0.45, 0.1, 0.005, 0.34, 0.25, 0.46, 0.42, 0.8, 0.65};
//     ivec locs(5);

//     findpeaks(Y, locs);
//     cout << "locs indexes = {" << locs(0) << ", " << locs(1) << ", " << locs(2) \
//          << ", " << locs(3) << ", " << locs(4) << "}" << endl;
//     return 0;
// }
