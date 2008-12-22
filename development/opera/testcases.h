// Testcases for task 5
// Sample inputs that show different types of symmetry
// Supplied as an array in an includable file for simplicity

static const char * testcases[] = {
// All symmetries
"ffffffff"
"ffffffff"
"ffffffff"
"ffffffff"
"ffffffff"
"ffffffff"
"ffffffff"
"ffffffff" ,

"tttttttt"
"tttttttt"
"tttttttt"
"tttttttt"
"tttttttt"
"tttttttt"
"tttttttt"
"tttttttt" ,

"tttttttt"
"ffffffff"
"tttttttt"
"ffffffff"
"ffffffff"
"tttttttt"
"ffffffff"
"tttttttt" ,

"tttttttt"
"tfffffft"
"tfffffft"
"tfffffft"
"tfffffft"
"tfffffft"
"tfffffft"
"tttttttt" ,

"tfffffft"
"ftfffftf"
"fftfftff"
"fffttfff"
"fffttfff"
"fftfftff"
"ftfffftf"
"tfffffft" ,

"fffttfff"
"fftfftff"
"ftfffftf"
"tfffffft"
"tfffffft"
"ftfffftf"
"fftfftff"
"fffttfff" ,

"fffttfff"
"fffttfff"
"fffttfff"
"tttttttt"
"tttttttt"
"fffttfff"
"fffttfff"
"fffttfff" ,

"ttfffftt"
"ttfffftt"
"ttfffftt"
"tttttttt"
"tttttttt"
"ttfffftt"
"ttfffftt"
"ttfffftt" ,

// Horizontal symmetry only
"ffffffff"
"ffffffff"
"ffffffff"
"ffffffff"
"tttttttt"
"tttttttt"
"tttttttt"
"tttttttt" ,

"ttfffftt"
"ttfffftt"
"ffttttff"
"ffttttff"
"ttfffftt"
"ttfffftt"
"ffttttff"
"ffttttff" ,

"tttttttt"
"fttttttf"
"ffttttff"
"fffttfff"
"tfffffft"
"ttfffftt"
"tttffttt"
"tttttttt" ,

"tfffffft"
"ftfffftf"
"fftfftff"
"fffttfff"
"tfffffft"
"ftfffftf"
"fftfftff"
"fffttfff" ,

"ttfffftt"
"ttfffftt"
"fttffttf"
"fttffttf"
"ffttttff"
"ffttttff"
"fffttfff"
"fffttfff" ,

"tfffffft"
"ttfffftt"
"fttffttf"
"ffttttff"
"fffttfff"
"fffttfff"
"fffttfff"
"fffttfff" ,

"ffttttff"
"fttffttf"
"fttffttf"
"ffttttff"
"fttttttf"
"ttfffftt"
"ttfffftt"
"ffttttff" ,

"fffttfff"
"ffttttff"
"fttffttf"
"ttfffftt"
"tttttttt"
"tttttttt"
"ttfffftt"
"ttfffftt" ,

// Vertical symmetry only
"ttttffff"
"ttttffff"
"ttttffff"
"ttttffff"
"ttttffff"
"ttttffff"
"ttttffff"
"ttttffff" ,

"ttffttff"
"ttffttff"
"ffttfftt"
"ffttfftt"
"ffttfftt"
"ffttfftt"
"ttffttff"
"ttffttff" ,

"fffffftt"
"ffffttff"
"ffttffff"
"ttffffff"
"ttffffff"
"ffttffff"
"ffffttff"
"fffffftt" ,

"tttttttt"
"tttttttf"
"ttttttff"
"ffffffff"
"ffffffff"
"ttttttff"
"tttttttf"
"tttttttt" ,

"ffttttff"
"fttttttf"
"ttfffftt"
"ttffffff"
"ttffffff"
"ttfffftt"
"fttttttf"
"ffttttff" ,

"ttttffff"
"ttttttff"
"ttfffttf"
"ttfffftt"
"ttfffftt"
"ttfffttf"
"ttttttff"
"ttttffff" ,

"tttttttt"
"tttttttt"
"ttffffff"
"tttttttt"
"tttttttt"
"ttffffff"
"tttttttt"
"tttttttt" ,

"ttttttff"
"ttfffftt"
"ttfffftt"
"ttttttff"
"ttttttff"
"ttfffftt"
"ttfffftt"
"ttttttff" ,

// No symmetry
"ttttffff"
"ttttffff"
"ttttffff"
"ttttffff"
"ffffffff"
"ffffffff"
"ffffffff"
"ffffffff" ,

"ftffffff"
"ffffffff"
"ffffffff"
"ffffffff"
"ffffffff"
"ffffffff"
"ffffffff"
"ffffffff" ,

"ftffffff"
"tffffttf"
"ftfffttf"
"tfffffff"
"ftttffff"
"fffttttf"
"fffffttf"
"tffftttf" ,

"ttfffttf"
"ffttffft"
"tfffttff"
"fttffftt"
"fffttfff"
"ttfffttf"
"ffttffft"
"tfffttff" ,

"tttttttt"
"tttttttt"
"fffffttf"
"ffffttff"
"fffttfff"
"ffttffff"
"fttfffff"
"ttffffff" ,

"tttttttt"
"tttttttt"
"ttffffff"
"tttttttt"
"tttttttt"
"ttffffff"
"ttffffff"
"ttffffff" ,

"ffttttff"
"fttttttf"
"ttfffftt"
"ttffffff"
"ttfftttt"
"ttffffft"
"tttttttf"
"ffttttff" ,

"fffffftt"
"fffffftt"
"fffffftt"
"fffffftt"
"fffffftt"
"ttfffftt"
"fttffttf"
"ffttttff" };
