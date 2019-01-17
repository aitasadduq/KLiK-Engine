<p align="center">
  <img src="_git%20assets/cover.png" width="600" align="center"/>
</p>

> The KLiK Engine is a C++ Powered File Search Engine for the Enron Email Sample Dataset

# Table of Contents

* [Features](#Features)
* [Components](#Components)
  * [Languages](#Languages)
  * [Dataset](#dataset)
  * [Development Environment](#Development-Environment)
  * [Database](#database)
  * [API](#api)
  * [Frameworks and Libraries](#Frameworks-and-Libraries)
* [Details](#details)
* [Future Improvements](#future-improvements)
* [The Team](#the-team)


## Features

* [Forward Indexer](#forward-indexer)
* [Reverse Indexer](#reverse-indexer)
* [Searcher](#searcher)
* [GUI](#gui)


## Components

#### Languages
```
C++
PHP 5.6.40
SQL 14.0
HTML5
CSS3
```

#### Dataset
* [Enron Email Dataset](https://enrondata.readthedocs.io/en/latest/)
* Size: 1.54 GB

#### Development Environment
```
Visual Studio 2017
WampServer Stack 3.0.6
Windows 10
```

#### Database
```
MySQL Database 8.0.13
```

#### API
```
MySQLx APIs
```

#### Frameworks and Libraries
```
C++ Boost Library
BootStrap v4.2.1
```

## Details

> Details of important Features of the Application

### Performance Analysis

* Forward Indexing:
```
300000 files/s
Incremental Processing (10000 files): 10 min
Total Time: 3hr
```
* Reverse Indexing
```
352000 files/s
Incremental Processing (10000 files): 15 min
Total Time: 2.4hr
```
*  Querying
```
Single Word Querying:   0.1 - 0.7 sec
Multi Word Querying:    0.4 - 2.3 sec
```

### Forward Indexer

* Implementation of `C++ Boost Library` to facilitate in I/O processes, since the dataset had many small files.
* Email Files loaded into memory at an `increment of 10000`, followed by mass processing of all loaded files. After that, the memory was freed and the process was started anew for the next 10000 files.
* `Stopping Words` filtered out of the email files
* Implementation of `MySQLx APIs` for SQL connections.
* Implementation of `unordered maps` for memory performance enhancement
* `Time calculation` of entire as well as the incremental processes.

### Reverse Indexer

* Implementation of forward Indexer for reverse index creation
* `Incremental File Processing` like in forward indexing.
* `Time Calculation` for the incremental and complete processes
* Implementation of `ranking` to ease in later searching
* Implementation of `Relevance Ranking`
* Implementation of `Search Normalization` to prevent misuse of the ranking system by too many same words in a common file.


### Searcher

* Implementation of `reverse index` in searching
* Calculation of `document score` and `inverse document score` for `relevance ranking`.
* Retrieval of `search query/string` from the `GUI`
* `Top 15` results returned from calculated search results.
* `Stopping words` safely removed from `search string`

  #### single word querying
  * `score` calculation of each result and  ordering in descending order.

  #### multi word querying
  * `score` of results concerning `key-words` belonging to same files `multiplied` to get common score.
  * implementation of `ordered maps` for automatic ordering of results with respect to their scores

### GUI

<p align='center'>
  <img src="_git%20assets/gui.png" width="550" align="center"/>
</p>

* Created in `PHP` / `HTML5` & `CSS3`
* implementation of `BootStrap4` Framework for a presentable interface
* Passing of input search query to the `C++ Searcher script` and receiving list of results as output.
* Display of all results with `email subject` as `title` along with the file path
* The result titles are `file links` redirecting to a new browser windows displaying all of the relevant `file content`.
* Implementation of `time calculation` on the GUI so user can see the `query time` as well

## Future Improvements
* Optimization (in components like indexing)
* Implementing of more advanced `indexing` and `ranking` algorithms
* Continuous Bug fixes and improvements

## The Team

A huge thanks to the wonderful team without which this entire project would not have been possible. Check out their profiles and star their repos! :)

<img src="_git%20assets/me.png" width="150"/> | <img src="_git%20assets/shaharyar.jpg" width="150"/> | <img src="_git%20assets/ahmed.jpg" width="150"/> | <img src="_git%20assets/ait.png" width="150"/>
---|---|---|---
[msaad1999](https://github.com/msaad1999) | [mshaharyar17](https://github.com/mshaharyar17) | ahmed | [aitasadduq](https://github.com/aitasadduq)
