For the navigation part:

Basically, my navigation function would traverse all the content in the current directory,
if it detect a directory, then it fork a child to do the navigation inside of the sub-directory
and record the pid into a pid list; if it detect a csv file, then it would fork a child to to
do the actual sort(your sort function goes here). After traversing all the contents in the current
directory, the parent would wait all its children whose pids are stored in temp_list. No out-sync need to be
considered since my temp_list is a local variable, which means any parent would only wait for its own child.
Also, since the function is bottom up, parents always wait for the child, no orphan or zombie possible

For the Extra credit part, I manually create a hierarchy tree that present all the parents and children relations.
Basically, parents are always present in the left side, and if they have children, their children would present on their right.
If a parent has more than one child, then I would leave enough rows between this pid and its siblings in order to
be able to store all its children.

We sort the CSV files using radix sort, treating the list of fields as a
vector and sorting the last component, then the second-to-last, and so on.
Since mergesort is stable, this is a valid implementation of radix sort. We
perform mergesort by first finding already sorted regions, then merging them
until they combine into a single region.
