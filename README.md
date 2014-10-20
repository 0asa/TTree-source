## TTree-source

## [Exploiting SNP correlations within Random Forest for Genome-Wide Association Studies](http://www.plosone.org/article/info%3Adoi%2F10.1371%2Fjournal.pone.0093379)

The primary goal of genome-wide association studies (GWAS) is to discover variants that could lead, in isolation or in combination, to a particular trait or disease. Standard approaches to GWAS however are usually based on univariate hypothesis tests and therefore can account neither for correlations due to linkage disequilibrium nor for combinations of several markers. To discover and leverage such potential multivariate interactions, we propose in this work an extension of the Random Forest algorithm tailored for structured GWAS data.

In terms of risk prediction, we show empirically on several GWAS datasets that the proposed *T-Trees* method significantly outperforms both the original Random Forest algorithm and standard linear models, thereby suggesting the actual existence of multivariate non-linear effects due to the combinations of several SNPs. We also demonstrate that variable importances as derived from our method can help identify relevant loci. Finally, we highlight the strong impact that quality control procedures may have, both in terms of predictive power and loci identification.

## Help

### Build

Run `make` in the TTree-source folder. That should do it.

Try running `./TTree -?` or the `example/example.sh` script.

### Input files

Input files are simple text files. There are 4 types (with some variations for bloc and set files) of input files.

- **JDB file**: one individual by row, each one described by a string (ignored by the TTree program but mandatory) followed by a 0 or a 1 (denoting the disease status of the individual) followed by the genotypes (encoded as 0, 1 or 2 - denoting the number of mutant alleles). In the [exemple file](https://raw.githubusercontent.com/0asa/TTree-source/master/example/s10-10d80-80-nb100-o10000-nn.jdb) you will note the presence of five `;` as a header, these are mandatory and give some room to add comment about the dataset if needed. 
- **Bloc file**: you have two options:

	- **start-end**: allow to represent blocks of contiguous SNPs in a compact way. Of course, this supposes that you ordered your genotypes according to their position on the genome. Note that start and end are represented as positions (starting at 0) in the JDB file not chromosomal bp position. With this option each row of the bloc file is denoted by two numbers tab separated. You can overlap blocks or play with blocks of different sizes if you need it. This is the default format. 
	- **indexes**: in this format each row contains a list of position, 
corresponding to the genotype you want to ‘group’ into blocks. This format 
is more flexible in a sense that you don’t need the SNP to be ordered in the 
JDB nor contiguous inside the blocks. To use this format you will have to 
run the TTree with the `-e` option. 
- **Learning and testing set files**: *[more to come]*
- **Candidate file**: *[more to come]*

### Output files

For a run, the six following output file are created (although in some case some of them might be empty):

- `.vim`: variable importances with first column for variable number, second column variable importance.
- `.gim`: group importances with first column for gorup number, second column group importance.
- `.roc`: contains all the necessary to compute ROC curves. First column is the object number, second column is the probability of the object belonging to class 1, third column is the real class.
- `.tree`: as your forest grows, this file details the evolution of `.roc` file (somehow)... You should not care about this unless you want to see how AUC evolves as the number of tree in your ensemble increases.
- `.dot`: dot file for the 10 first trees in your ensemble. You can/should be open the dot file using [GraphViz](http://www.graphviz.org) or using the dot command such as: `dot input.dot -Tpng > output.png`. This is meant to give you an overview of what the trees look like (might be buggy).
- `.log`: contains many informations about the current run (date, parameters used, learning and test set used, ...). 

### Options

*[more to come]*
 
```
$ ./TTree -?

TTree Pack 1.0 (vince.bot@gmail.com)
Options:
	Required fields
	---------------
	 -j <jdbfile>
	 -m <0|1|2|3|4>
	 -t <nbTree>
	 -k <nbRand>
	Optional fields
	---------------
	 -b <blocfile>
	 -s for snp datasets (optimized code will be used)
	 -r <seed> to specify a seed (>-1) (if not specified will be time based)
	 -f for full logs
	 -i <increment> increment for output files
	 -v <testsetfile>
	 -l <learintsetfile>
	 -a <attcandfile>
	 -g <groupcandfile>
	 -n <nmin>
	 -c <internalcomplexity>
	 -e not start end file for blockfile
	 -x not start end file for ts/ls file
	 -? this message...
```	 