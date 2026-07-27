## TTree-source

## [利用随机森林中的 SNP 相关性进行全基因组关联研究](http://www.plosone.org/article/info%3Adoi%2F10.1371%2Fjournal.pone.0093379)

全基因组关联研究 (GWAS) 的主要目标是发现单独或组合在一起可能导致特定性状或疾病的变异。然而，标准的 GWAS 方法通常基于单变量假设检验，因此既不能解释由于连锁不平衡引起的相关性，也无法解释多个标记的组合。为了发现并利用这种潜在的多变量交互作用，我们在本研究中提出了一种针对结构化 GWAS 数据量身定制的随机森林 (Random Forest) 算法扩展。

在风险预测方面，我们在多个 GWAS 数据集上通过实证研究表明，所提出的 *T-Trees* 方法显著优于原始的随机森林算法和标准线性模型，从而表明由于多个 SNP 的组合而确实存在多变量非线性效应。我们还证明，通过该方法导出的变量重要性 (variable importances) 有助于识别相关位点。最后，我们强调了质量控制程序在预测能力和位点识别方面的强大影响。

## 帮助

### 构建

在 TTree-source 文件夹中运行 `make` 即可。

尝试运行 `./TTree -?` 或 `example/example.sh` 脚本。

### 输入文件

输入文件是简单的文本文件。共有 4 种类型（bloc 文件和 set 文件有一些变体）。

- **JDB 文件**：每一行代表一个个体，首先是一个字符串（由 TTree 程序忽略但必须提供），接着是一个 0 或 1（表示个体的疾病状态），然后是基因型（编码为 0, 1 或 2 - 表示突变等位基因的数量）。在 [示例文件](https://raw.githubusercontent.com/0asa/TTree-source/master/example/s10-10d80-80-nb100-o10000-nn.jdb) 中，你会注意到开头有五个 `;` 作为文件头，这些是必须的，并可在需要时用于添加关于数据集的注释。
- **Bloc 文件**：你有两个选项：

	- **start-end**：允许以紧凑的方式表示连续 SNP 块。当然，这假设你已经根据基因组位置对基因型进行了排序。请注意，start 和 end 表示的是在 JDB 文件中的位置（从 0 开始），而非染色体 bp 位置。在这种选项下，bloc 文件的每一行由两个由制表符分隔的数字表示。如果需要，你可以重叠块或使用不同大小的块。这是默认格式。
	- **indexes**：在这种格式中，每一行包含一个位置列表，对应于你想要“分组”成块的基因型。这种格式更灵活，因为你不需要 JDB 中的 SNP 有序，也不需要它们在块内连续。要使用此格式，必须在运行 TTree 时带有 `-e` 选项。
- **学习集和测试集文件 (Learning and testing set files)**：*[更多内容即将推出]*
- **候选文件 (Candidate file)**：*[更多内容即将推出]*

### 输出文件

运行一次后，会创建以下六个输出文件（但在某些情况下，其中一些可能为空）：

- `.vim`：变量重要性，第一列为变量编号，第二列为变量重要性。
- `.gim`：组重要性，第一列为组编号，第二列为组重要性。
- `.roc`：包含计算 ROC 曲线所需的所有内容。第一列是对象编号，第二列是该对象属于类别 1 的概率，第三列是真实类别。
- `.tree`：随着森林的增长，该文件详细记录了 `.roc` 文件的演变（在某种程度上）... 除非你想查看 AUC 随集成中树数量增加而如何演变，否则无需关注此文件。
- `.dot`：集成中前 10 棵树的 dot 文件。你可以/应该使用 [GraphViz](http://www.graphviz.org) 或使用 dot 命令打开该文件，例如：`dot input.dot -Tpng > output.png`。这旨在让你概览树的外观（可能存在 bug）。
- `.log`：包含有关当前运行的许多信息（日期、使用的参数、使用的学习集和测试集...）。

### 选项

*[更多内容即将推出]*

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
