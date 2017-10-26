For the navigation part:

Basically, my navigation function would traverse all the content in the current directory,
if it detect a directory, then it fork a child to do the navigation inside of the sub-directory
and record the pid into a pid list; if it detect a csv file, then it would fork a child to to 
do the actual sort(your sort function goes here). After traversing all the contents in the currenty 
directory, the parent would wait all its children whose pids are stored in temp_list. No out-sync nned to be 
considered since my temp_list is a local variable, which means any parent would only wait for its own child. 
Also, since the function is bottom up, parents always wait for the child, no orphan or zombie possible

Note, I add global varibale sort_num to record the number of fileds on which our sort should be based.
I changed the second argument in sort_csv function to a double pointer type since we sometimes need to sort
serveral fileds. I add another argument output_path in sort_csv function since we may want to output all the 
file into a specific directory;
  
