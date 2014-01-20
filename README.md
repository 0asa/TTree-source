TTree-source
============

Exploiting SNP correlations within Random Forest for Genome-Wide Association Studies

The primary goal of genome-wide association studies (GWAS) is to discover variants that could lead, in isolation or in combination, to a particular trait or disease. Standard approaches to GWAS however are usually based on univariate hypothesis tests and therefore can account neither for correlations due to linkage disequilibrium nor for combinations of several markers. To discover and leverage such potential multivariate interactions, we propose in this work an extension of the Random Forest algorithm tailored for structured GWAS data.

In terms of risk prediction, we show empirically on several GWAS datasets that the proposed *T-Trees* method significantly outperforms both the original Random Forest algorithm and standard linear models, thereby suggesting the actual existence of multivariate non-linear effects due to the combinations of several SNPs. We also demonstrate that variable importances as derived from our method can help identify relevant loci. Finally, we highlight the strong impact that quality control procedures may have, both in terms of predictive power and loci identification.
