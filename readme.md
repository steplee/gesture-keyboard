# Keyboard

Gesture keyboard like Swype, GBoard, etc.

 - /frontend has a simple js canvas display
 - /layouts defines keyboard layouts
 - /external has dependencies. Use `./download_deps.sh` to get them.



## Immediate TODO
 1. Switch `Thread` to use smart pointer children.
 2. Implement deletion & dying of threads.
 3. 

## Working notes

Trie\<V\> is a datastructure for char -> V\*, but I am not currently using V;

Threads (concurrency) are used only in main.cc, all other uses of the word `Thread` refer to the search system.

#### Scoring
There are several ideas I have from pairwise potentials, RNNs, CNNs, SVM on resampled path, etc.

###### Pairwise Potentials
Right now I am working on pairwise edge potentials. A word is built by sequential pairs, each which factorize to a full joint
score/probability. To prevent combinatorial explosion, I will have a search facilitating candidate words to be scored.
For example if we see `a -> b -> c -> d -> e`, we can search for `abe` by searching
for the best path from `a -> b` and then `b -> e` and multiplying their potentials.

This is sort of an implicit DP by the fact that all data is pairwise.
This also has the advantage of being unsupervised.


#### Invalidating old nodes
Instead of having a normal softmax, incorporate an *age penalty* $\frac{e^{-(s_i + (\text{finish} / \text{now}))}}{\textbb{Z}}$
Besides addition, a multiplication might work better.

#### Removing invalid branches
If a thread has no children and is old enough, it will delete itself.
When a thread 'dies', it should notify its parent and cascade. They keep track of alive children.
TODO: also, if distinct turns have been made, we should age a thread more.

## General reminders
  - Time does not necessarily mean time, in fact I am leaning towards it mean accumulated distance.
  - For most cases, you need a *delayed* scoring system. You don't want to score a move when the next char comes into
    some radius around the path, but when it is in the center.
     - One way to do this might to use a `just_created` flag in the thread, and when querying, **re-evaluate its potential**.
  - Any thread should have at most one child of the same character, so it's safe to have a 26-sized array.
  - In the future, take into account acceleration and filtering techniques.
  - In my search terminology, 'dead' != 'delete'
