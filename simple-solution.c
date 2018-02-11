#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>   // Used for kmalloc
#include <linux/list_sort.h>   // Used to sort list


//Define Structure
struct birthday {
   int year;
   int month;
   int day;
   const char *name;
   struct list_head list;
};

/**
 * The following defines and initializes a list_head object named birthday_list
 */
static LIST_HEAD(birthday_list);

//  Method to create and add a person to birthday_list
void addPerson(char *name, int day, int month, int year) {
   struct birthday *person;

   person = kmalloc(sizeof(*person), GFP_KERNEL); // allocate memory for person
   person->day = day;
   person->month = month;
   person->year = year;
   person->name = name;

   INIT_LIST_HEAD(&person->list); // initiate list head
   list_add_tail(&person->list, &birthday_list); // tail add list head to birthday List
}

// Method to print each element of linkedlist to Kernel
void print(void) {
   struct birthday *ptr;  //  ptr used in list_for_each_entry macro to keep track of position
   list_for_each_entry(ptr, &birthday_list, list) {
      printk(KERN_INFO "Name: [%s]\tBirthday: [%d/%d/%d]\n", ptr->name, ptr->month, ptr->day, ptr->year);
   }
}

// Compare method used to sort list based on highest year (Youngest to Oldest)
int compare(void *priv, struct list_head *a, struct list_head *b) {
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
   int i = 1;
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

   // Sorts by age
   list_sort(test, &birthday_list, compare);

   print();

   printk(KERN_NOTICE "Removing youngest...\n");

   // Deletes first element of list
   list_for_each_entry_safe(ptr, next, &birthday_list, list) {
      if (i == 1) {
         list_del(&ptr->list);
         i = 0;
      }
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
