# ToDo-CLI: A Simple Command-Line Task Tracker

This project provides a basic command-line interface (CLI) for managing your tasks. It’s a simple yet effective tool for keeping track of to-dos, marking them as completed, and organizing your workflow.

***Features***

*  _Adding New Tasks:_ Easily create new tasks with the add command.
*  _Updating and Deleting Tasks:_ Modify existing tasks with update and remove them with delete.
*  _Marking Tasks as In Progress or Done:_ Use mark-in-progress and mark-done to track task status.
*  _Listing Tasks:_ Display all tasks with list, or filter by status with done, todo, or in-progress.

***Usage***

```
#Adds a new task
ToDo-CLI add "Buy groceries"  

#Updates the task with ID 1
ToDo-CLI update 1 "Buy groceries and cook dinner"  

#Deletes the task with ID 1
ToDo-CLI delete 1  

#Marks the task with ID 1 as in progress
ToDo-CLI mark-in-progress 1

#Marks the task with ID 1 as done
ToDo-CLI mark-done 1  

#Lists all tasks
ToDo-CLI list  

#Lists completed tasks
ToDo-CLI done 

# Lists pending tasks
ToDo-CLI todo  

# Lists tasks in progress
ToDo-CLI in-progress
```

***Getting Started***

**Clone the repository:**

git clone https://github.com/DioXmoi/ToDo-CLI.git

**Navigate to the project directory:**
cd ToDo-CLI

**Compile and run:**
#Replace "make" with your preferred build system (e.g., cmake, qmake)

make
./ToDo-CLI 
