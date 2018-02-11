/***  Yoel Elias
  *   CSC 4320 Project 1
  *   Birthday Kernel Mod
  *   This project will use the built in kernel linked list to create and display a birthday list
  *   At mod install it will create a list, sort the list based by age, with the Youngest at the top, and remove the youngest
  *   At mod remove it will delete the list and free kernel memory
***/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>   // Used for kmalloc
#include <linux/list_sort.h>   // Used to sort list


/***  Birthday Structure
  *   This struct will keep track of a person's birth year, day, and month
  *   It will also hold the list_head struct to be used in kernel linked list
***/
struct birthday {
   int year;
   int month;
   int day;
   const char *name;
   struct list_head list;
};

/***
  * The following defines and initializes a list_head object named birthday_list
***/
static LIST_HEAD(birthday_list);

/***
  *   Function: addPerson
  *   --------------------
  *   Creates a pointer to a birthday struct
  *   allocates kernel memory
  *   Initializes struct values
  *   Adds the person to LIST_HEAD at the tail using list_add_tail macro
  *
  *   *name: pointer of desired person's name
  *   day: day of birth
  *   month: month of birth
  *   year: year of birth
  *
  *   Returns: Nothing
***/
void addPerson(char *name, int day, int month, int year)
{
   struct birthday *person;

   person = kmalloc(sizeof(*person), GFP_KERNEL); // allocate memory for person
   person->day = day;
   person->month = month;
   person->year = year;
   person->name = name;

   INIT_LIST_HEAD(&person->list); // initiate list head
   list_add_tail(&person->list, &birthday_list); // tail add list head to birthday List
}

/***
  *   Function: print
  *   -----------------
  *   Creates a birthday pointer for macro to use
  *   Uses list_for_each_entry macro to print to kernel
  *   Print format example: Name: [Test]  Birthday: [Month/day/Year]
  *   
  *   Returns: Nothing
***/
void print(void)
{
   struct birthday *ptr;  //  ptr used in list_for_each_entry macro to keep track of position
   list_for_each_entry(ptr, &birthday_list, list) 
   {
      printk(KERN_INFO "Name: [%s]\tBirthday: [%d/%d/%d]\n", ptr->name, ptr->month, ptr->day, ptr->year);
   }
}

/***
  *   Function: compare
  *   --------------------
  *   Compares two members of list_head via year, month, day
  *   Used in list_sort macro
  *   Creates temp1 and temp2 to compare birthdat struct values
  *   
  *   Returns:
  *   1 if temp2 is greater than temp1 (via year, month, day)
  *   -1 if temp1 is greater than temp 2 (via year, month, day)
  *   0 if they are exactly the same
***/
int compare(void *priv, struct list_head *a, struct list_head *b)
{
   struct birthday *temp1, *temp2;
   temp1 = kmalloc(sizeof(*temp1), GFP_KERNEL);
   temp2 = kmalloc(sizeof(*temp2), GFP_KERNEL);

   temp1 = list_entry(a, struct birthday, list);
   temp2 = list_entry(b, struct birthday, list);

   // Comparing by year
   if (temp1->year < temp2->year) {
      return 1;
   }
   else if (temp1->year > temp2->year) {
      return -1;
   }
   else {
      // Comparing by month
      if (temp1->month < temp2->month) {
         return 1;
      }
      else if (temp1->month > temp2->month) {
         return -1;
      }
      else {
         // Comparing by day
         if (temp1->day < temp2->day) {
            return 1;
         }
         else if (temp1->day > temp2->day) {
            return -1;
         }
            else {
               return 0;
            }
      }
   }
}


// Kernel load
int simple_init(void) {
   struct birthday *ptr, *next;
   void *test = NULL; // Used in sorting macro

   printk(KERN_INFO "Loading Module...\n");

   // adds six people
   addPerson("Yoel", 6, 7, 1995);
   addPerson("Salma", 3, 3, 1992);
   addPerson("Jelado", 21, 12, 1997);
   addPerson("Person", 25, 12, 1800);
   addPerson("Luiggi", 14, 2, 1980);
   addPerson("Marita", 4, 4, 1985);

   printk(KERN_NOTICE "Displaying Birthdays...\n");

   print();

   printk(KERN_NOTICE "After sorting...\n");

   // list_sort marco uses defined comparions above to sort by age
   list_sort(test, &birthday_list, compare);

   print();

   printk(KERN_NOTICE "Removing youngest...\n");

   // Deletes first element of list
   list_for_each_entry_safe(ptr, next, &birthday_list, list) {
      list_del(&ptr->list);
      kfree(ptr);
      break;
   }

   print();

   return 0;
}

// Kernal removal
void simple_exit(void) {

   struct birthday *ptr, *next;
   printk(KERN_INFO "Removing Module...\n");

   list_for_each_entry_safe(ptr, next, &birthday_list, list) {

      printk(KERN_NOTICE "Deleting: [%s]\n", ptr->name);
      list_del(&ptr->list);
      kfree(ptr);
   }

   printk(KERN_INFO "Done\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Data Structures");
MODULE_AUTHOR("Yoel Elias");
