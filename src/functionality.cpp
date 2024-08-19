#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <map>
#include <sstream>
#include <cstring>

//define the vetor of the tasks: 
//1. store the information inside the txt file 
//2. show the welcome page 
 
enum MenuOption {
    ShowTodayTasks = 0,
    AddTask,
    RemoveTask,
    MarkAsFinished,
    MarkAsUnfinished,
    DeleteAll,
    Leave
};

char PASSWORD[100] = "cyz030518";

const int WAIT_TIME = 500;

// Function to load tasks from a file
void load_tasks(std::map<std::string, std::vector<std::pair<std::string, bool>>>& tasks, const std::string& filename, WINDOW *menu_win) {
    // std::cout << "start to load the file" << std::endl;
    std::ifstream file(filename);
    std::string line;
    if (file.is_open()) {
        // std::cout << "start to load the file1" << std::endl;
        while (getline(file, line)) {
            // std::cout << "start to load the fistd::make_pair<std::string(task),false>le2" << std::endl;

            std::size_t pos = line.find(':');  // Find the position of the colon
            if (pos != std::string::npos) {
                // std::cout << "start to load the file3" << std::endl;

                std::string key = line.substr(0, pos);
                // std::cout << "start to load the file4" << std::endl;
                // std::cout << "the key is: "<< key << std::endl;
                // std::cout << "the CURR_TIME is: "<< CURR_TIME << std::endl;
                // std::cout << "start to load the file5" << std::endl;
                // mvwprintw(menu_win, 20, 1, "load the file successfully ");

                // std::cout<<"this is the correct message" << std::endl;
                std::string valueString = line.substr(pos + 1);
                std::vector<std::pair<std::string, bool>> taskList;
                std::stringstream ss(valueString);
                std::string item;
                while (getline(ss, item, ',')) {
                    std::size_t semicolon_pos = item.find(';');
                    if (semicolon_pos != std::string::npos) {
                        // Extract the task name and the boolean value as strings
                        std::string task_name = item.substr(0, semicolon_pos);
                        std::string bool_str = item.substr(semicolon_pos + 1);

                        // Convert the boolean string to a boolean value
                        bool task_status = (bool_str == "1" || bool_str == "true");

                        // Add the task and its status to the task list
                        taskList.push_back(std::make_pair(task_name, task_status));
                    }
                }
                tasks[key] = taskList;
                
                 
                 
            }
            else{
                // std::cout<<"this is the wrong message" << std::endl;
            }
        }
        file.close();
    }
    else if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for saving tasks." << std::endl;
        return;
    }
   
}

// Function to save tasks to a file
void save_tasks(std::map<std::string, std::vector<std::pair<std::string, bool>>>& tasks,  const std::string& filename) {
    std::cout<< "start to save the task" << std::endl;
    std::ofstream file(filename);
    if (file.is_open()){
        for (const auto& pair : tasks) {
            file << pair.first << ":";
            for (size_t i = 0; i < pair.second.size(); ++i) {
                file << pair.second[i].first;
                file << ";";
                file << pair.second[i].second;
                if (i < pair.second.size() - 1) {
                    file << ",";
                }
            }
            file << "\n";
        }
    }
    else if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for saving tasks." << std::endl;
        return;
    }
}

// Function to display the current date
std::string get_current_date() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char date[10];
    sprintf(date, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    return std::string(date);
}

// Function to add a new task
void add_task(WINDOW *input_win, std::map<std::string, std::vector<std::pair<std::string, bool>>>& tasks, std::string CURR_TIME ) {
    char task[100];
    wclear(input_win);
    box(input_win, 0,0);
    mvwprintw(input_win, 1, 1, "Enter new task: ");  // 将光标移动到输入窗口中的 (1, 1) 位置，并显示 "Enter new task: " 提示用户输入任务。
    wrefresh(input_win);

   
    wgetnstr(input_win, task, sizeof(task) - 1);
   
    tasks[CURR_TIME].push_back(std::pair<std::string, bool>(task, false));

    wclear(input_win);
    // wrefresh(input_win);
    // delwin(input_win);

}

// Function to display the welcome page
enum MenuOption show_welcome_page(WINDOW *menu_win, std::vector<std::string> menu) {
    wclear(menu_win);
    int choice = 0;
    
    box(menu_win,0,0);
    mvwprintw(menu_win, 1, 1, "Yuzhench Time Schedule");
    mvwprintw(menu_win, 2, 1, ("Today's Date: " + get_current_date()).c_str());
    mvwprintw(menu_win, 4, 1, "Options you can choose:");

    int y = 5;
    for (int i = 0; i < menu.size(); ++i) {
        mvwprintw(menu_win, y++, 1, "%d. %s", i + 1, menu[i].c_str());
    }

    mvwprintw(menu_win, y + 1, 1, "Enter your choice (1-%d): ", menu.size());  // 提示用户选择

    wrefresh(menu_win);  // 刷新窗口

    choice = getch() - '0';  // 获取用户输入并转换为数字

    if (choice >= 1 && choice <= menu.size()) {
        return static_cast<MenuOption>(choice - 1);  // 用户输入有效，退出循环
    } 
    else {
        mvwprintw(menu_win, y + 2, 1, "Invalid choice! Please try again.");  // 无效选择提示
        wrefresh(menu_win);  // 刷新窗口
    }

}


void mark_as_finished(WINDOW *input_win, std::map<std::string, std::vector<std::pair<std::string, bool>>>& tasks, std::string CURR_TIME ){
    char task_index_char[100];
    box(input_win,0,0);
    mvwprintw(input_win,1,1, "enter the finished task index: ");
    wrefresh(input_win);

    wgetnstr(input_win, task_index_char, sizeof(task_index_char) - 1);
    int task_index_int = std::stoi(task_index_char) - 1;


    wclear(input_win);
    box(input_win,0,0);
    wrefresh(input_win);

    if (task_index_int >= 0 && task_index_int < tasks[CURR_TIME].size()){
        mvwprintw(input_win,1,1, ("make task " + std::string(task_index_char) + " finished").c_str());
        tasks[CURR_TIME][task_index_int].second = true;
    }
    else{
        mvwprintw(input_win,1,1, "invalid task index");
    }
    wrefresh(input_win);
    napms(WAIT_TIME);

}

 

void show_tasks(WINDOW *task_win, std::map<std::string, std::vector<std::pair<std::string, bool>>>& tasks, std::string CURR_TIME) {
    wclear(task_win);
    box(task_win,0,0);
    mvwprintw(task_win, 1, 1,(get_current_date() + " Tasks:").c_str());

    if (tasks[CURR_TIME].size() == 0){
        mvwprintw(task_win, 5,1,"--current is empty--");
    }

    int y = 3;

    mvwprintw(task_win,1,35, "total_taskes: %d", tasks[CURR_TIME].size());

    for (int i = 0; i < tasks[CURR_TIME].size(); ++i) {
        mvwprintw(task_win, y++, 1, "%d. %s", i + 1, tasks[CURR_TIME][i].first.c_str());
        if (tasks[CURR_TIME][i].second){
            mvwprintw(task_win, y - 1, 20, "%s", "finished");
        }
        else{
            mvwprintw(task_win, y - 1, 20, "%s", "non-finished");
        }
         
    }
    wrefresh(task_win);
    // getch();  // Wait for user input before returning to menu
}

void delet_today_tasks (WINDOW *input_element_win, std::map<std::string, std::vector<std::pair<std::string, bool>>>& tasks, std::string CURR_TIME){
    char password[100];
    box(input_element_win, 0,0);
    mvwprintw(input_element_win, 1, 1, "permission checking, please input your password");
    wmove(input_element_win, 4, 1); 
    wrefresh(input_element_win);

    wgetnstr(input_element_win, password, sizeof(password) - 1);

    if (std::strcmp(password, PASSWORD) == 0) {
        tasks[CURR_TIME].clear();
        wclear(input_element_win);
        box(input_element_win, 0,0);
        wrefresh(input_element_win);
        mvwprintw(input_element_win, 1, 1, "delete all tasks");
    } else {
        wclear(input_element_win);
        box(input_element_win, 0,0);
        wrefresh(input_element_win);
        mvwprintw(input_element_win, 1, 1, "you don't have the permission to delete all the tasks");
    }
    wrefresh(input_element_win);
    napms(WAIT_TIME);
    wclear(input_element_win);
    // napms(3000);
    // wrefresh(input_element_win);

}

// Function to delete a task
void delete_one_task(WINDOW *input_element_win,  std::map<std::string, std::vector<std::pair<std::string, bool>>>& tasks, std::string CURR_TIME) {
    char task_num[100];
    box(input_element_win, 0,0);
    mvwprintw(input_element_win,1, 1, "Delete which task: ");
    wrefresh(input_element_win);

    wgetnstr(input_element_win, task_num, sizeof(task_num) - 1);
    int task_index = std::stoi(task_num);
    
    if (task_index >= 1 && task_index <= tasks[CURR_TIME].size()) {
        tasks[CURR_TIME].erase(tasks[CURR_TIME].begin() + task_index - 1);
        wclear(input_element_win);
        wrefresh(input_element_win);
        box(input_element_win, 0,0);
        mvwprintw(input_element_win,1, 1, ("deleted task: " + std::to_string(task_index)).c_str());
         
    } else {
        wclear(input_element_win);
        wrefresh(input_element_win);
        box(input_element_win, 0,0);
        mvwprintw(input_element_win,1, 1, "you type the wrong task index! ");
         
    }
    wrefresh(input_element_win);
    napms(WAIT_TIME);
    wclear(input_element_win);
    wrefresh(input_element_win);
}

int main(int argc, char **argv) {
    std::cout <<"Hellow world" << std::endl;
    std::map<std::string, std::vector<std::pair<std::string, bool>>> tasks;
    // tasks["2024-08-18"] = {"task1", "task2"};

    std::vector<std::string> menu = {"Show today tasks", "Add a task", "Remove a task", "Mark as finished", "Mark as unfinished", "Delete today tasks", "Leave"};

    std::string get_current_date(); 
    std::string CURR_TIME = get_current_date();

     


    // Initialize ncurses
    initscr();
    // clear();
    // noecho();
    // cbreak();


    //define the size of the window
    int height = LINES;  
    int width = COLS / 2;

    // std::cout << "height is: " << height << std::endl;
     // 定义窗口的宽高
    int win_height = 10;
    int win_width = 40;

    // 计算窗口的起始位置，使其居中
    int starty = (height - win_height) / 2;
    int startx = (width * 2 - win_width) / 2;

    WINDOW *input_element_win = newwin(win_height, win_width, starty, startx);

    WINDOW *menu_win = newwin(height, width, 0, 0);  
    WINDOW *task_win = newwin(height, width, 0, width); 
     
   

    refresh();

    //ready to get the choice
    int choice = 0;

    bool running = true;

    //load the file:
    std::string filePath = std::string(DATA_DIR) + "/history.txt";
    load_tasks(tasks, filePath, menu_win);

    while(running){
        enum MenuOption choice = show_welcome_page(menu_win, menu);  // 显示菜单   
        wrefresh(menu_win);

        // load_tasks(tasks, "src/history.txt", CURR_TIME);


        switch (choice) {
            case ShowTodayTasks:
                show_tasks(task_win, tasks, CURR_TIME);
                wrefresh(task_win);
                break;
            case AddTask:
                add_task(input_element_win, tasks, CURR_TIME);
                wrefresh(input_element_win);

                //update the task
                show_tasks(task_win, tasks, CURR_TIME);
                wrefresh(task_win);
                break;
            case RemoveTask:
                delete_one_task(input_element_win, tasks, CURR_TIME);
                wrefresh(menu_win); // 确保 menu_win 在窗口删除前刷新
                show_tasks(task_win, tasks, CURR_TIME);
                wrefresh(task_win); // 确保 task_win 在窗口删除前刷新
                break;
            case MarkAsFinished:
                mark_as_finished(input_element_win, tasks, CURR_TIME);
                wrefresh(menu_win); // 确保 menu_win 在窗口删除前刷新
                show_tasks(task_win, tasks, CURR_TIME);
                wrefresh(task_win); // 确保 task_win 在窗口删除前刷新
                break;
            case MarkAsUnfinished:
                std::cout << "Marking a task as unfinished." << std::endl;
                break;
            case DeleteAll:
                delet_today_tasks(input_element_win,tasks, CURR_TIME);
                refresh();
                wrefresh(menu_win); // 确保 menu_win 在窗口删除前刷新
                show_tasks(task_win, tasks, CURR_TIME);
                wrefresh(task_win); // 确保 task_win 在窗口删除前刷新

                // delwin(input_element_win); // 删除窗口
                // input_element_win = nullptr; // 确保指针被设置为nullptr防止悬挂引用
                break;
            case Leave:
                // save_tasks(tasks, "src/history.txt");
                running = false;
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
                break;
        }
        
    }
     
    //  // Refresh to display changes
    // refresh();

    // // Wait for user input before exiting
    // getch();

    // std::string detail = tasks["2024-08-18"][0];
    // std::cout << "tasks is: " << detail <<std::endl;

    save_tasks(tasks, filePath);
    // End ncurses mode
    endwin();

    
    
    return 0;
}
