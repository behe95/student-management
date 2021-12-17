#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <sstream>
#include <windows.h>
#include <string>

using namespace std;

enum ACCOUNT_TYPES
{
    TEACHER = 1,
    STUDENT = 2
};

enum TABLE_HEADERS_CRED
{
    FIRST_NAME = 0,
    LAST_NAME = 1,
    USER_NAME = 2,
    PASSWORD = 3,
    STUDENT_ENROLL_STATUS = 4
};

enum STUDENT_ENROLL_STATUS
{
    NOT_ENROLLED = 0,
    ENROLLED = 1
};

enum STUDENT_ENROLLED_TABLE_HEADERS
{
    STUDENT_USERNAME = 0,
    TEACHER_USERNAME = 1,
    MATH,
    PHYSICS,
    CHEMISTRY
};

const string TEACHER_CRED_FILE_NAME = "teacher_credentials.txt";
const string STUDENT_CRED_FILE_NAME = "student_credentials.txt";
const string TEMP_FILE = "temp.txt";
const string ENROLLED_STUDENT_DATA_FILE_NAME = "enrolled.txt";

const string NOT_GRADED = "Not Graded";

//function prototype to show main menu
//return type void
//arguments none
void show_main_menu_screen();
//function prototype for sign up
//return type void
//arguments none
void sign_up();
//function prototype for sign in
//return type void
//arguments none
void sign_in();

//function prototype for removing extra data from the stream
//return type void
//arguments none
void ignore_stream_cin();

//function prototypes for console text color attributes
//return type void
//argument none
void get_default_color();
void get_primary_color();
void get_secondary_color();
void get_success_color();
void get_danger_color();
void get_warning_color();
void get_dark_color();

//function prototype for showing account types menu
//return type void
//arguments none
void show_account_types_menu();

void show_teacher_dashboard();
void show_student_dashboard();

void show_profile();
void show_students_list();
void option_selection_for_student_data_modification(vector<vector<string>>* student_list, vector<int>* column_widths);
void assign_marks(vector<vector<string>>* student_list, vector<int>* column_widths);
void update_marks(vector<vector<string>>* student_list, vector<int>* column_widths);
void delete_student(vector<vector<string>>* student_list, vector<int> column_widths);
void show_teachers_list();
void enroll_student(vector<vector<string>> teachers_list);
void log_out();

void calculate_width_of_table_columns(int total_rows, int total_columns, vector<vector<string>> datas, vector<int>* column_widths);
void draw_table_cells(int total_rows, int total_columns, vector<vector<string>> datas, vector<int> column_widths);
void draw_horizontal_line(vector<int> column_widths, char line_type);


//CONSOLE_SCREEN_BUFFER_INFOR is a struct which stores information about console screen buffer
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

//Declaring and assigning a handle
// a handle refer to some object which is a unique value
// later we can use this handle to track something
// in this case which is the output screen or the console window
//STD_OUTPUT_HANDLE is a constant variable to refer to the CONSOLE SCREEN
HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

//declaring 2d vector of string type
//for storing teachers and students login credentials
vector<vector<string>> teachers_cred_vect;
vector<vector<string>> students_cred_vect;

//string type variable
//to store account type i.e. student account, teacher account etc.
int account_type;

//declaring boolean type variable
//assigning false value by default
//this variable will be used as a flag
//to determine if a student or a teacher is logged in or not
bool USER_LOGGED_IN = false;

//declaring a vector of string type
//to store logged in user info at the time of sign in authorization
//first name
//last name
//user name
vector<string> logged_in_user_info_vec;

//main function where everything begins
int main()
{
    //this function gets the buffer information of the console
    //passed the console window handle and the memory address location of the csbiInfo variable (CONSOLE_SCREEN_BUFFER_INFO type)
    //this function will use the handle to retrieve all the information and will store it in the memory address of csbiInfo variable
    //the function return 0 for fail and no-zero for success
    //so to get the information we have to pass the memory address of the csbiInfo
    GetConsoleScreenBufferInfo(hStdout, &csbiInfo);




    //using infinite while loop to run the
    //program forver
    while(true)
    {
        //calling the function
        //to show the main menu screen
        show_main_menu_screen();

        while(USER_LOGGED_IN)
        {
            if(account_type == ACCOUNT_TYPES::TEACHER)
                show_teacher_dashboard();
            else if(account_type == ACCOUNT_TYPES::STUDENT)
                show_student_dashboard();
        }
        system("cls");

    }
    return 0;
}

//function body for showing main  menu screen on the console
void show_main_menu_screen()
{
    //defining an enum
    //to refer the sign up and sign in option
    enum choices
    {
        SIGN_UP = 1,
        SIGN_IN = 2,
    };

    //int type variable
    //to store users selected input
    int selected_choice;

    //function call to change the console text color
    get_secondary_color();

    //hardcoded to show option on the console screen
    cout << "|================|" << endl
         << "|    Main Menu   |" << endl
         << "|================|" << endl
         << "|   1. Sign Up   |" << endl
         << "|----------------|" << endl
         << "|   2. Sign In   |" << endl
         << "|================|" << endl << endl;

    //changing the text color
    get_warning_color();
    cout << "Select an option from the above. Use any positive integer number corresponding to each item\t";

    //infinite loop
    //this loop will run until the user selected a valid input
    //otherwise the loop will break
    while(true)
    {
        //asking user for input
        cin >> selected_choice;

        get_default_color();

        //cin.rdstate() method checks the state/status of the cin input stream
        //the return type of cin.rdstate() method is int type
        //it can return any one of the values from 0 to 4
        //We will only only look at return value 0 and 4 here. Check the documentation for the meaning of the value 1,2 and 3;
        //the return value 0 means that there is no error in cin input stream. We can also say that 0 means success
        //the return value 4 means that there is something wrong with the cin input stream. This state/status occurs when the cin input stream doesn't get
        //the expected input
        //For example here the cin is assigning input data in the "songsRequested" variable. As this variable is a float data type it can hold only numeric value.
        //So the cin input stream is expecting any numeric value.
        //But if the user input any non-numeric value i.e. "A", "B", "Foo", "Bar", "@@!@" etc, which is not being expected by the cin input stream, then there will occur some malfunction in the cin input stream.
        //We cannot work with cin input stream any more. Such as we cannot get any input using cin or cannot perform any method on cin such as cin.ignore() etc.
        //As there is an error, the cin.rdstate() will return 4 and the following condition will be executed
        // but if cin input stream got numeric value then cin.rdstate() would return the value 0;
        //At this time the cin input stream will stop working properly unless this malfunction has been fixed.
        //So to fix this we need to clear the malfunctioning of the cin input stream. That's why we are running cin.clear() method.
        //As we have fixed our cin input stream using cin.clear() method now we can work with cin again.
        //At this time we need to remove the non-numeric values that caused the malfunction because cin.clear() only fixed the problem of cin input stream. It didn't remove
        //the values that caused the problem. Those problem causing values are still in the input stream. So we are going to run cin.ignore() method to remove those values.
        if(cin.rdstate() == 4)
        {
            cin.clear();
            //cin.ignore() function is written inside the following function body
            //separated this function for repetition
            ignore_stream_cin();
        }

        //this switch case is going to validate the user input
        //then it will call the appropriate function
        //according to the user input
        switch(selected_choice)
        {
            case choices::SIGN_UP:
                sign_up();
                break;
            case choices::SIGN_IN:
                sign_in();
                break;
            default:
                get_danger_color();
                cout << "Input is out of range. Try again\t";
                continue;
        }
        break;
    }

}

//function to execute sign up operation
void sign_up()
{
    //clearing console screen
    system("cls");



    //calling the function
    //to show the account types on the screen
    //this function also asks the user to select account type
    //validate the user input
    show_account_types_menu();

    //clearing console screen
    system("cls");

    get_secondary_color();
    cout << "|================================================|" << endl
         << "|                 Input Rules                    |" << endl
         << "|================================================|" << endl
         << "|    o First Name Cannot Contain Any Space       |" << endl
         << "|------------------------------------------------|" << endl
         << "|    o Last Name Cannot Contain Any Space        |" << endl
         << "|------------------------------------------------|" << endl
         << "|    o User Name Cannot Contain Any Space        |" << endl
         << "|------------------------------------------------|" << endl
         << "|    o Password Cannot Contain Any Space         |" << endl
         << "|------------------------------------------------|" << endl
         << "|    x Any extra spaces will be removed          |" << endl
         << "|------------------------------------------------|" << endl
         << "|    x Anything after a spaces will be removed   |" << endl
         << "|================================================|" << endl << endl;


    //string type variable to store file name
    string file_name;

    //depending on the account type we will use different file
    //to store different account type's credentials
    if(account_type == ACCOUNT_TYPES::TEACHER)
        file_name = TEACHER_CRED_FILE_NAME;
    else if(account_type == ACCOUNT_TYPES::STUDENT)
        file_name = STUDENT_CRED_FILE_NAME;

    //declaring and opening input file stream
    ifstream input_file(file_name);
    //declaring and opening output file stream and also we are passing "ios::app" flag
    //so that we can append the file instead of replacing the file
    ofstream output_file(file_name, ios::app);




    //enum to refer table headers
    enum TABLE_HEADERS
    {
        FIRST_NAME = 0,
        LAST_NAME = 1,
        USER_NAME = 2,
        PASSWORD = 3
    };

    //boolean variable
    //to check if a username already exists in our database
    bool user_exists = false;



    //if the file exists the this condition will execute
    if(input_file.good())
    {
        //running a while loop and extracting datas from the filestream
        //until we reach to the end of the file
        while(!input_file.eof())
            {
                //this variable will store each line from the filestream temporarily
                string temp_data = "";
                //we will separate temp_data by comma delimeter
                //and we will store those multiple datas in the
                //temporary vector
                vector<string> temp_vector;

                //get lines from input file stream and store into temp_data variable
                getline(input_file, temp_data);

                //declaring a stringstream to convert the temp_data string into stream
                stringstream temp_stream(temp_data);

                //now we will separate datas from string stream by comma delimeter
                //we will store push those data to the temporary vector
                while(getline(temp_stream, temp_data, ','))
                {
                    temp_vector.push_back(temp_data);
                }

                //depending on the account type we will store the temp vector to the original vector
                if(account_type == ACCOUNT_TYPES::TEACHER)
                    teachers_cred_vect.push_back(temp_vector);
                else if(account_type == ACCOUNT_TYPES::STUDENT)
                    students_cred_vect.push_back(temp_vector);
            }

            //as we are done with the input file stream
            //now we can close the connection
            input_file.close();

    }

    //string variables to store user information
    string first_name, last_name, user_name, password;

    //this infinite loop will run until it gets non duplicated user name
    //other it will break
    while(true)
    {
        get_warning_color();
        cout << "First Name\t";
        cin >> first_name;
        ignore_stream_cin();
        cout << endl;

        get_success_color();
        cout << "Last Name\t";
        cin >> last_name;
        ignore_stream_cin();
        cout << endl;

        get_warning_color();
        cout << "User Name\t";
        cin >> user_name;
        ignore_stream_cin();
        cout << endl;

        get_success_color();
        cout << "Password\t";
        cin >> password;
        ignore_stream_cin();
        cout << endl;


        if(account_type == ACCOUNT_TYPES::TEACHER && teachers_cred_vect.size() > 0)
        {
            for(int i = 0; i < teachers_cred_vect.size(); i++)
            {
                if(teachers_cred_vect[i][TABLE_HEADERS::USER_NAME] == user_name)
                {
                    user_exists = true;
                    get_danger_color();
                    cout << "User with the same username already exists!" << endl;
                    break;
                }
            }
            if(user_exists)
            {
                user_exists = false;
                continue;
            }
        }
        else if(account_type == ACCOUNT_TYPES::STUDENT && students_cred_vect.size() > 0)
        {
            for(int i = 0; i < students_cred_vect.size(); i++)
            {
                if(students_cred_vect[i][TABLE_HEADERS::USER_NAME] == user_name)
                {
                    user_exists = true;
                    get_danger_color();
                    cout << "User with the same username already exists!" << endl;
                    break;
                }
            }
            if(user_exists)
            {
                user_exists = false;
                continue;
            }
        }

        if(!user_exists)
        {
            break;
        }
    }
    if(account_type == ACCOUNT_TYPES::TEACHER && teachers_cred_vect.size() > 0)
        output_file << endl;
    else if(account_type == ACCOUNT_TYPES::STUDENT && students_cred_vect.size() > 0)
        output_file << endl;
    output_file << first_name << ","
                    << last_name << ","
                    << user_name << ","
                    << password;
    if(account_type == ACCOUNT_TYPES::STUDENT)
        output_file << "," << STUDENT_ENROLL_STATUS::NOT_ENROLLED;

    output_file.close();

    if(account_type == ACCOUNT_TYPES::TEACHER)
        teachers_cred_vect.clear();
    else if(account_type == ACCOUNT_TYPES::STUDENT)
        students_cred_vect.clear();

    get_success_color();
    system("cls");
    cout << "User registration completed successfully!!" << endl;
    get_default_color();
}

void sign_in()
{
    system("cls");
    cout << "SIGN IN" << endl;





    show_account_types_menu();



    string file_name;

    if(account_type == ACCOUNT_TYPES::TEACHER)
        file_name = TEACHER_CRED_FILE_NAME;
    else if(account_type == ACCOUNT_TYPES::STUDENT)
        file_name = STUDENT_CRED_FILE_NAME;

    ifstream input_file(file_name);




    if(input_file.good())
    {
        while(!input_file.eof())
            {
                string temp_data = "";
                vector<string> temp_vector;

                getline(input_file, temp_data);
                stringstream temp_stream(temp_data);

                while(getline(temp_stream, temp_data, ','))
                {
                    temp_vector.push_back(temp_data);
                }
                if(account_type == ACCOUNT_TYPES::TEACHER)
                    teachers_cred_vect.push_back(temp_vector);
                else if(account_type == ACCOUNT_TYPES::STUDENT)
                    students_cred_vect.push_back(temp_vector);
            }
            input_file.close();

    }

    string user_name, password;

    while(true)
    {
        get_warning_color();
        cout << "User Name\t";
        cin >> user_name;
        ignore_stream_cin();
        cout << endl;

        get_success_color();
        cout << "Password\t";
        cin >> password;
        ignore_stream_cin();
        cout << endl;

        if(account_type == ACCOUNT_TYPES::TEACHER && teachers_cred_vect.size() > 0)
        {
            for(int i = 0; i < teachers_cred_vect.size(); i++)
            {
                if(teachers_cred_vect[i][TABLE_HEADERS_CRED::USER_NAME] == user_name && teachers_cred_vect[i][TABLE_HEADERS_CRED::PASSWORD] == password)
                {
                    USER_LOGGED_IN = true;
                    logged_in_user_info_vec.push_back(teachers_cred_vect[i][TABLE_HEADERS_CRED::FIRST_NAME]);
                    logged_in_user_info_vec.push_back(teachers_cred_vect[i][TABLE_HEADERS_CRED::LAST_NAME]);
                    logged_in_user_info_vec.push_back(teachers_cred_vect[i][TABLE_HEADERS_CRED::USER_NAME]);
                    get_danger_color();
                    cout << "User logged in with username ";
                    get_warning_color();
                    cout << user_name << endl;
                    break;
                }
            }
            if(!USER_LOGGED_IN)
            {
                cout << "Invalid user name or password! Try again" << endl;
                continue;
            }
        }
        else if(account_type == ACCOUNT_TYPES::STUDENT && students_cred_vect.size() > 0)
        {
            for(int i = 0; i < students_cred_vect.size(); i++)
            {
                if(students_cred_vect[i][TABLE_HEADERS_CRED::USER_NAME] == user_name && students_cred_vect[i][TABLE_HEADERS_CRED::PASSWORD] == password)
                {
                    USER_LOGGED_IN = true;
                    logged_in_user_info_vec.push_back(students_cred_vect[i][TABLE_HEADERS_CRED::FIRST_NAME]);
                    logged_in_user_info_vec.push_back(students_cred_vect[i][TABLE_HEADERS_CRED::LAST_NAME]);
                    logged_in_user_info_vec.push_back(students_cred_vect[i][TABLE_HEADERS_CRED::USER_NAME]);
                    logged_in_user_info_vec.push_back("");
                    logged_in_user_info_vec.push_back(students_cred_vect[i][TABLE_HEADERS_CRED::STUDENT_ENROLL_STATUS]);
                    get_danger_color();
                    cout << "User logged in with username !" << user_name << endl;
                    break;
                }
            }
            if(!USER_LOGGED_IN)
            {
                cout << "Invalid user name or password! Try again" << endl;
                continue;
            }
        }

        if(!USER_LOGGED_IN)
        {
            cout << "User doesn't exist! Try to sign up and login again!" << endl;
        }

        if(USER_LOGGED_IN)
        {
            if(account_type == ACCOUNT_TYPES::TEACHER)
                teachers_cred_vect.clear();
            else if(account_type == ACCOUNT_TYPES::STUDENT)
                students_cred_vect.clear();
            break;
        }
    }



}

//List of color functions

/*
0   BLACK
1   BLUE
2   GREEN
3   CYAN
4   RED
5   MAGENTA
6   BROWN
7   LIGHTGRAY
8   DARKGRAY
9   LIGHTBLUE
10  LIGHTGREEN
11  LIGHTCYAN
12  LIGHTRED
13  LIGHTMAGENTA
14  YELLOW
15  WHITE
*/

void get_default_color()
{
    SetConsoleTextAttribute(hStdout, 15);
}
void get_primary_color()
{
    SetConsoleTextAttribute(hStdout, 9);
}
void get_secondary_color()
{
    SetConsoleTextAttribute(hStdout, 3);
}
void get_success_color()
{
    SetConsoleTextAttribute(hStdout, 10);
}
void get_danger_color()
{
    SetConsoleTextAttribute(hStdout, 4);
}
void get_warning_color()
{
    SetConsoleTextAttribute(hStdout, 6);
}
void get_dark_color()
{
    SetConsoleTextAttribute(hStdout, 8);
}

//cin.ignore() method going to ignore any remaining data on the stream
//the first argument decides how many data we want to ignore. And in this case numeric_limits<streamsize)::max() which is a static method says that we want
//to remove maximum data
//and then the second argument decides we want the maximum data to be just before we find any '\n'
//for example: Suppose the user put "1a" as the input for package and then press "Enter". In another term "Enter" is actually a newline character or '\n'
//so when the user press "Enter" the package variable will accept only the first character from "1a". Because "1a" is a string literal and the package variable
//is a char data type. So package variable will accept the first byte and ignore the rest of the bytes from the stream.
//But the other bytes will still remain in the stream. In this case "a" will be remained in the stream.
//Now the validation will be failed because there is no package called "1" as we have only package 'A', 'B' and 'C'.
//So the while loop will run again
//But this time the user will not get any change to input for the package. Because there is already some remaining input data in the stream
//in this case which is 'a' and this data will be stored to the package variable.
//so we don't want this to be happened. And that is why we want to remove the extra bytes from the stream until we get '\n'
void ignore_stream_cin()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


void show_account_types_menu()
{


    int selected_choice;
    get_secondary_color();
    cout << "|==================|" << endl
         << "|   Account Type   |" << endl
         << "|==================|" << endl
         << "|    1. Teacher    |" << endl
         << "|------------------|" << endl
         << "|    2. Student    |" << endl
         << "|==================|" << endl << endl;

    get_warning_color();
    cout << "Select an option from the above. Use any positive integer number corresponding to each item\t";

    while(true)
    {
        cin >> selected_choice;

        get_default_color();

        if(cin.rdstate() == 4)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch(selected_choice)
        {
            case ACCOUNT_TYPES::TEACHER:
                account_type = ACCOUNT_TYPES::TEACHER;
                break;
            case ACCOUNT_TYPES::STUDENT:
                account_type = ACCOUNT_TYPES::STUDENT;
                break;
            default:
                get_danger_color();
                cout << "Input is out of range. Try again\t";
                continue;
        }
        break;

    }
}

void show_teacher_dashboard()
{
    system("cls");
    //defining an enum
    //to refer the sign up and sign in option
    enum choices
    {
        SHOW_PROFILE = 1,
        SHOW_STUDENTS,
        LOG_OUT
    };

    //int type variable
    //to store users selected input
    int selected_choice;

    //function call to change the console text color
    get_secondary_color();

    //hardcoded to show option on the console screen
    cout << "|======================|" << endl
         << "|        Main Menu     |" << endl
         << "|======================|" << endl
         << "|   1. Show Profile    |" << endl
         << "|----------------------|" << endl
         << "|   2. Show Students   |" << endl
         << "|----------------------|" << endl
         << "|   3. Logout          |" << endl
         << "|======================|" << endl << endl;

    //changing the text color
    get_warning_color();
    cout << "Select an option from the above. Use any positive integer number corresponding to each item\t";

    cin >> selected_choice;

    switch(selected_choice)
    {
        case choices::SHOW_PROFILE:
            show_profile();
            break;
        case choices::SHOW_STUDENTS:
            show_students_list();
            break;
        case choices::LOG_OUT:
            log_out();
            break;
        default:
            cout << "Input is out of range. Try again\t";
    }
}

void show_student_dashboard()
{
    system("cls");
    //defining an enum
    //to refer the sign up and sign in option
    enum choices
    {
        SHOW_PROFILE = 1,
        SHOW_TEACHERS,
        LOG_OUT
    };

    //int type variable
    //to store users selected input
    int selected_choice;

    //function call to change the console text color
    get_secondary_color();

    //hardcoded to show option on the console screen
    cout << "|======================|" << endl
         << "|        Main Menu     |" << endl
         << "|======================|" << endl
         << "|   1. Show Profile    |" << endl
         << "|----------------------|" << endl
         << "|   2. Show Teachers   |" << endl
         << "|----------------------|" << endl
         << "|   3. Logout          |" << endl
         << "|======================|" << endl << endl;

    //changing the text color
    get_warning_color();
    cout << "Select an option from the above. Use any positive integer number corresponding to each item\t";

    cin >> selected_choice;

    switch(selected_choice)
    {
        case choices::SHOW_PROFILE:
            show_profile();
            break;
        case choices::SHOW_TEACHERS:
            show_teachers_list();
            break;
        case choices::LOG_OUT:
            log_out();
            break;
        default:
            cout << "Input is out of range. Try again\t";
    }
}

void show_profile()
{
    string file_name;

    if(account_type == ACCOUNT_TYPES::TEACHER)
        file_name = TEACHER_CRED_FILE_NAME;
    else if(account_type == ACCOUNT_TYPES::STUDENT)
        file_name = STUDENT_CRED_FILE_NAME;

    vector<vector<string>> cell_datas = {{"First Name", logged_in_user_info_vec[TABLE_HEADERS_CRED::FIRST_NAME]},
                                    {"Last Name", logged_in_user_info_vec[TABLE_HEADERS_CRED::LAST_NAME]},
                                    {"User Name", logged_in_user_info_vec[TABLE_HEADERS_CRED::USER_NAME]}};
    vector<int> column_widths;

    calculate_width_of_table_columns(cell_datas.size(), cell_datas[0].size(), cell_datas, &column_widths);


    cout << endl
             << "********************* Student Info ********************" << endl
             << endl;
    draw_horizontal_line(column_widths, '=');
    draw_table_cells(cell_datas.size(), cell_datas[0].size(), cell_datas, column_widths);
    draw_horizontal_line(column_widths, '=');

    if(account_type == ACCOUNT_TYPES::STUDENT && stoi(logged_in_user_info_vec[TABLE_HEADERS_CRED::STUDENT_ENROLL_STATUS]) == STUDENT_ENROLL_STATUS::NOT_ENROLLED)
        cout << endl
             << "********************* You are currently not enrolled to any teacher ********************" << endl
             << "************************ Select a teacher from the Teachers List ***********************" << endl
             << endl;
    else if(account_type == ACCOUNT_TYPES::STUDENT && stoi(logged_in_user_info_vec[TABLE_HEADERS_CRED::STUDENT_ENROLL_STATUS]) == STUDENT_ENROLL_STATUS::ENROLLED)
    {
        vector<vector<string>> student_course_info = {{"Teacher", "Math", "Physics", "Chemistry"}};

        ifstream input_file_student_enrolled_info(ENROLLED_STUDENT_DATA_FILE_NAME);
        string temp_string = "";
        vector<string> temp_vect;

        ifstream teachers_cred_file(TEACHER_CRED_FILE_NAME);
        vector<string> temp_vect_teacher_cred;

        if(input_file_student_enrolled_info)
        {
            while(!input_file_student_enrolled_info.eof())
            {
                getline(input_file_student_enrolled_info, temp_string);
                stringstream temp_stream(temp_string);

                while(getline(temp_stream, temp_string, ','))
                {
                    temp_vect.push_back(temp_string);
                }
                if(temp_vect[STUDENT_ENROLLED_TABLE_HEADERS::STUDENT_USERNAME] == logged_in_user_info_vec[TABLE_HEADERS_CRED::USER_NAME])
                {


                    while(!teachers_cred_file.eof())
                    {
                        getline(teachers_cred_file, temp_string);
                        stringstream temp_stream_teacher(temp_string);

                        while(getline(temp_stream_teacher, temp_string, ','))
                        {
                            temp_vect_teacher_cred.push_back(temp_string);
                        }
                        if(temp_vect[STUDENT_ENROLLED_TABLE_HEADERS::TEACHER_USERNAME] == temp_vect_teacher_cred[TABLE_HEADERS_CRED::USER_NAME])
                            break;
                        else
                            temp_vect_teacher_cred.clear();
                    }
                    temp_vect[STUDENT_ENROLLED_TABLE_HEADERS::TEACHER_USERNAME] = temp_vect_teacher_cred[TABLE_HEADERS_CRED::FIRST_NAME] + " " + temp_vect_teacher_cred[TABLE_HEADERS_CRED::LAST_NAME];
                    temp_vect.erase(temp_vect.begin() + STUDENT_ENROLLED_TABLE_HEADERS::STUDENT_USERNAME);
                    student_course_info.push_back(temp_vect);
                    break;
                }
            }
        }
        else
        {
            cout << "ERROR!!Something went wrong!!" << endl;
        }

        input_file_student_enrolled_info.close();
        teachers_cred_file.close();

        vector<int> column_widths_for_student_enroll_info;
        calculate_width_of_table_columns(student_course_info.size(), student_course_info[0].size(), student_course_info, &column_widths_for_student_enroll_info);

        cout << endl
             << "********************* Course Info ********************" << endl
             << endl;
        draw_horizontal_line(column_widths_for_student_enroll_info, '=');
        draw_table_cells(student_course_info.size(), student_course_info[0].size(), student_course_info, column_widths_for_student_enroll_info);
        draw_horizontal_line(column_widths_for_student_enroll_info, '=');
    }




    system("pause");


}

void show_students_list()
{
    enum STUDENT_LIST_TABLE_HEADRES_INDEX
    {
        SERIAL_NUMBER = 0,
        FIRST_NAME,
        LAST_NAME,
        USER_NAME,
        MATH,
        PHYSICS,
        CHEMISTRY
    };
    vector<vector<string>> cell_datas = {{"Serial Number", "First Name", "Last Name", "User Name", "Math", "Physics", "Chemistry"}};
    vector<int> column_widths;


    ifstream input_file_enrolled_student(ENROLLED_STUDENT_DATA_FILE_NAME);
    ifstream input_file_student_creds(STUDENT_CRED_FILE_NAME);

    if(input_file_enrolled_student.good())
    {

        int index = 0;
        string temp_data = "";
        vector<string> temp_vector;
        vector<vector<string>> students_course_datas_for_current_logged_in_user;
        vector<vector<string>> students_creds_for_current_logged_in_user;


        while(!input_file_enrolled_student.eof())
        {
            index++;

            vector<string> temp_vector_for_student_course_data;

            temp_vector.push_back(to_string(index));

            getline(input_file_enrolled_student, temp_data);
            stringstream temp_stream(temp_data);

            while(getline(temp_stream, temp_data, ','))
            {
                temp_vector_for_student_course_data.push_back(temp_data);
            }

            if(temp_vector_for_student_course_data[STUDENT_ENROLLED_TABLE_HEADERS::TEACHER_USERNAME] == logged_in_user_info_vec[TABLE_HEADERS_CRED::USER_NAME])
                students_course_datas_for_current_logged_in_user.push_back(temp_vector_for_student_course_data);

        }


        if(students_course_datas_for_current_logged_in_user.size() > 0)
        {
            while(!input_file_student_creds.eof())
            {
                getline(input_file_student_creds, temp_data);
                stringstream temp_stream_for_student_cred(temp_data);
                vector<string> temp_vector_for_student_cred;

                while(getline(temp_stream_for_student_cred, temp_data, ','))
                {
                    temp_vector_for_student_cred.push_back(temp_data);
                }

                for(auto course_info:students_course_datas_for_current_logged_in_user)
                {
                    if(course_info[STUDENT_ENROLLED_TABLE_HEADERS::STUDENT_USERNAME] == temp_vector_for_student_cred[TABLE_HEADERS_CRED::USER_NAME])
                    {
                        for(int i = TABLE_HEADERS_CRED::FIRST_NAME; i <= TABLE_HEADERS_CRED::USER_NAME; i++)
                            temp_vector.push_back(temp_vector_for_student_cred[i]);

                        for(int i = STUDENT_ENROLLED_TABLE_HEADERS::MATH; i <= STUDENT_ENROLLED_TABLE_HEADERS::CHEMISTRY; i++)
                            temp_vector.push_back(course_info[i]);
                    }
                }
                cell_datas.push_back(temp_vector);
            }



            input_file_enrolled_student.close();
            input_file_student_creds.close();

            calculate_width_of_table_columns(cell_datas.size(), cell_datas[0].size(), cell_datas, &column_widths);

            draw_horizontal_line(column_widths, '=');
            draw_table_cells(cell_datas.size(), cell_datas[0].size(), cell_datas, column_widths);
            draw_horizontal_line(column_widths, '=');



        }
        else
        {
            cell_datas.clear();
            cout << endl
             << "********************* Sorry There is No Students Enrolled For Your Course ********************" << endl
             << "******************************** Try Again Later When Available ******************************" << endl
             << endl;
        }

    }
    else
    {
        cell_datas.clear();
        cout << endl
             << "********************* Sorry There is No Students Enrolled For Your Course ********************" << endl
             << "******************************** Try Again Later When Available ******************************" << endl
             << endl;

    }

    if(cell_datas.size() > 0)
        option_selection_for_student_data_modification(&cell_datas, &column_widths);

    system("pause");

}

void show_teachers_list()
{
    vector<vector<string>> cell_datas = {{"Serial Number", "First Name", "Last Name", "User Name"}};
    vector<int> column_widths;
    string file_name = TEACHER_CRED_FILE_NAME;

    ifstream input_file(file_name);

    if(input_file.good())
    {
        int index = 0;
        while(!input_file.eof())
            {
                index++;
                string temp_data = "";
                vector<string> temp_vector;

                temp_vector.push_back(to_string(index));

                getline(input_file, temp_data);
                stringstream temp_stream(temp_data);

                while(getline(temp_stream, temp_data, ','))
                {
                    temp_vector.push_back(temp_data);
                }
                cell_datas.push_back(temp_vector);
            }
            input_file.close();

            calculate_width_of_table_columns(cell_datas.size(), cell_datas[0].size(), cell_datas, &column_widths);

            draw_horizontal_line(column_widths, '=');
            draw_table_cells(cell_datas.size(), cell_datas[0].size(), cell_datas, column_widths);
            draw_horizontal_line(column_widths, '=');

            if(logged_in_user_info_vec[TABLE_HEADERS_CRED::STUDENT_ENROLL_STATUS] == to_string(STUDENT_ENROLL_STATUS::NOT_ENROLLED))
            {
                enroll_student(cell_datas);
            }



    }
    else
    {
        cout << endl
             << "********************* Sorry There is No Teacher Available in this School ********************" << endl
             << "******************************** Try Again Later When Available *****************************" << endl
             << endl;

    }

    system("pause");

}

void enroll_student(vector<vector<string>> teachers_list)
{
    int selected_teacher;
    vector<vector<string>> student_cred_cell_datas;

    cout << "Select a teacher from the above to enroll. Select any positive non-zero integer number\t";
    cin >> selected_teacher;

    string temp_file_name = TEMP_FILE;
    string file_name = STUDENT_CRED_FILE_NAME;
    ifstream input_file_student_cred(file_name);
    ofstream output_file_student_cred;

    while(!input_file_student_cred.eof())
    {
        string temp_data = "";
        vector<string> temp_vector;

        getline(input_file_student_cred, temp_data);
        stringstream temp_stream(temp_data);

        while(getline(temp_stream, temp_data, ','))
        {
            temp_vector.push_back(temp_data);
        }
        if(temp_vector[TABLE_HEADERS_CRED::USER_NAME] == logged_in_user_info_vec[TABLE_HEADERS_CRED::USER_NAME])
        {
            temp_vector[TABLE_HEADERS_CRED::STUDENT_ENROLL_STATUS] = to_string(STUDENT_ENROLL_STATUS::ENROLLED);
            logged_in_user_info_vec[TABLE_HEADERS_CRED::STUDENT_ENROLL_STATUS] = to_string(STUDENT_ENROLL_STATUS::ENROLLED);
            student_cred_cell_datas.push_back(temp_vector);
        }
        else
            student_cred_cell_datas.push_back(temp_vector);
    }
    input_file_student_cred.close();

    remove(file_name.c_str());

    output_file_student_cred.open(temp_file_name, ios::app);

    for(int row = 0; row < student_cred_cell_datas.size(); row++)
    {
        for(int column = 0; column < student_cred_cell_datas[row].size(); column++)
        {
            if(column == 0 && row != 0)
                output_file_student_cred << "\n";

            output_file_student_cred << student_cred_cell_datas[row][column];

            if(column != student_cred_cell_datas[row].size() - 1)
                output_file_student_cred << ",";
        }
    }

    output_file_student_cred.close();
    rename(temp_file_name.c_str(), file_name.c_str());

    ofstream output_file_enrolled_student;
    output_file_enrolled_student.open(ENROLLED_STUDENT_DATA_FILE_NAME, ios::app);

    if(!output_file_enrolled_student)
    {
        cout << "ERROR: Something went wrong! Could not open the file" << endl;
    }
    else
    {
        bool data_exists_in_enrolled_students_file = false;
        string temp_data = "";

        ifstream input_file_enrolled_students(ENROLLED_STUDENT_DATA_FILE_NAME);

        if(input_file_enrolled_students)
        {
            getline(input_file_enrolled_students, temp_data);
            if(temp_data.size() > 0)
                data_exists_in_enrolled_students_file = true;
        }
        input_file_enrolled_students.close();

        if(data_exists_in_enrolled_students_file)
            output_file_enrolled_student << endl;

        output_file_enrolled_student << logged_in_user_info_vec[TABLE_HEADERS_CRED::USER_NAME] << ",";
        output_file_enrolled_student << teachers_list[selected_teacher][TABLE_HEADERS_CRED::USER_NAME+1] << ",";
        output_file_enrolled_student << NOT_GRADED << ",";
        output_file_enrolled_student << NOT_GRADED << ",";
        output_file_enrolled_student << NOT_GRADED;

        output_file_enrolled_student.close();

        cout << "Enrolled to " << teachers_list[selected_teacher][TABLE_HEADERS_CRED::FIRST_NAME+1] << " "
             << teachers_list[selected_teacher][TABLE_HEADERS_CRED::LAST_NAME+1] << "'s class successfully!" << endl;

    }
}

void option_selection_for_student_data_modification(vector<vector<string>>* student_list, vector<int>* column_widths)
{
    while(true)
    {
        system("cls");
        draw_horizontal_line(*column_widths, '=');
        draw_table_cells(student_list->size(), student_list->at(0).size(), *student_list, *column_widths);
        draw_horizontal_line(*column_widths, '=');

        cout << "|============================|" << endl
             << "|           Option           |" << endl
             << "|============================|" << endl
             << "|   1. Assign Marks          |" << endl
             << "|----------------------------|" << endl
             << "|   2. Update Marks          |" << endl
             << "|----------------------------|" << endl
             << "|   3. Remove Student        |" << endl
             << "|----------------------------|" << endl
             << "|   4. Return To Dashboard   |" << endl
             << "|============================|" << endl << endl;

        cout << "Select any option from the above. Select only non-zero integer within above range!\t";

        int selected_option;

        cin >> selected_option;

        cout << endl;
        enum SELECT_OPTION
        {
            ASSIGN_MARKS = 1,
            UPDATE_MARKS,
            REMOVE_STUDENT,
            RETURN_TO_DASHBOARD
        };

        switch(selected_option)
        {
            case SELECT_OPTION::ASSIGN_MARKS:
                assign_marks(student_list, column_widths);
                break;
            case SELECT_OPTION::UPDATE_MARKS:
                update_marks(student_list, column_widths);
                break;
            case SELECT_OPTION::REMOVE_STUDENT:
                delete_student(student_list, *column_widths);
                break;
            case SELECT_OPTION::RETURN_TO_DASHBOARD:
                break;
            default:
                cout << "Invalid input out of range! Try again" << endl;
        }

        if(selected_option == SELECT_OPTION::RETURN_TO_DASHBOARD)
            break;
    }
}

void assign_marks(vector<vector<string>>* student_list, vector<int>* column_widths)
{
    cout << "Select a student by his/her serial number to assign marks. Select only non-zero integer number.\t";

    int selected_student;

    while(true)
    {
        cin >> selected_student;
        if(cin.rdstate() == 4)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "You cannot write any symbol or letter! Try again.\t";
        }
        else if(selected_student > student_list->size() - 1 || selected_student <= 0)
        {
            cout << "Invalid input out of range. Try again\t";
        }
        else
        {
            cout << endl;
            break;
        }

    }

    enum STUDENT_LIST_TABLE_HEADRES_INDEX
    {
        SERIAL_NUMBER = 0,
        FIRST_NAME,
        LAST_NAME,
        USER_NAME,
        MATH,
        PHYSICS,
        CHEMISTRY
    };

    int math_number, physics_number, chemistry_number;

    cout << "Selected Student:\t" << student_list->at(selected_student)[STUDENT_LIST_TABLE_HEADRES_INDEX::FIRST_NAME] << " "
         << student_list->at(selected_student)[STUDENT_LIST_TABLE_HEADRES_INDEX::LAST_NAME] << endl;

    cout << "Math Marks:\t";
    cin >> math_number;
    cout << endl;

    cout << "Physics Narks:\t";
    cin >> physics_number;
    cout << endl;

    cout << "Chemistry Marks:\t";
    cin >> chemistry_number;
    cout << endl;

    student_list->at(selected_student)[STUDENT_LIST_TABLE_HEADRES_INDEX::MATH] = to_string(math_number);
    student_list->at(selected_student)[STUDENT_LIST_TABLE_HEADRES_INDEX::PHYSICS] = to_string(physics_number);
    student_list->at(selected_student)[STUDENT_LIST_TABLE_HEADRES_INDEX::CHEMISTRY] = to_string(chemistry_number);

    vector<vector<string>> modified_student_datas;
    ifstream input_file(ENROLLED_STUDENT_DATA_FILE_NAME);
    ofstream output_file(TEMP_FILE, ios::app);

    while(!input_file.eof())
    {
        string temp_data = "";
        vector<string> temp_vector;

        getline(input_file, temp_data);
        stringstream temp_stream(temp_data);

        while(getline(temp_stream, temp_data, ','))
        {
            temp_vector.push_back(temp_data);
        }
        if(temp_vector[STUDENT_ENROLLED_TABLE_HEADERS::STUDENT_USERNAME] == student_list->at(selected_student)[STUDENT_LIST_TABLE_HEADRES_INDEX::USER_NAME])
        {
            temp_vector[STUDENT_ENROLLED_TABLE_HEADERS::MATH] = to_string(math_number);
            temp_vector[STUDENT_ENROLLED_TABLE_HEADERS::PHYSICS] = to_string(physics_number);
            temp_vector[STUDENT_ENROLLED_TABLE_HEADERS::CHEMISTRY] = to_string(chemistry_number);
            modified_student_datas.push_back(temp_vector);
        }
        else
            modified_student_datas.push_back(temp_vector);
    }
    input_file.close();

    remove(ENROLLED_STUDENT_DATA_FILE_NAME.c_str());

    for(int row = 0; row < modified_student_datas.size(); row++)
    {
        for(int column = 0; column < modified_student_datas[row].size(); column++)
        {
            if(column == 0 && row != 0)
                output_file << "\n";

            output_file << modified_student_datas[row][column];

            if(column != modified_student_datas[row].size() - 1)
                output_file << ",";
        }
    }

    output_file.close();

    rename(TEMP_FILE.c_str(), ENROLLED_STUDENT_DATA_FILE_NAME.c_str());

    cout << endl;
    cout << "Marks assigned to " << student_list->at(selected_student)[STUDENT_LIST_TABLE_HEADRES_INDEX::FIRST_NAME] << " "
         << student_list->at(selected_student)[STUDENT_LIST_TABLE_HEADRES_INDEX::LAST_NAME] << " successfully!" << endl;



}
void update_marks(vector<vector<string>>* student_list, vector<int>* column_widths)
{

}
void delete_student(vector<vector<string>>* student_list, vector<int> column_widths)
{

}

void log_out()
{
    USER_LOGGED_IN = false;
    logged_in_user_info_vec.clear();
}

void calculate_width_of_table_columns(int total_rows, int total_columns, vector<vector<string>> datas, vector<int>* column_widths)
{
    if(column_widths->size() == 0)
    {
        for(int i = 0; i < total_columns; i++)
            column_widths->push_back(0);
    }
    for(int row = 0; row < total_rows; row++)
    {
        for(int column = 0; column < total_columns; column++)
        {

            int length_of_string = datas[row][column].size();
            if(length_of_string > column_widths->at(column))
            {
                column_widths->at(column) = length_of_string+5;
            }
        }
    }
}

void draw_horizontal_line(vector<int> column_widths, char line_type)
{
    for(int i = 0; i < column_widths.size(); i++)
    {
        cout << "|";
        cout << string(column_widths.at(i), line_type);
        cout << "|";
    }
    cout << endl;
}

void draw_table_cells(int total_rows, int total_columns, vector<vector<string>> datas, vector<int> column_widths)
{
    for(int row = 0; row < total_rows; row++)
    {
        for(int column = 0; column < total_columns; column++)
        {
            string cell_data = datas[row][column];
            int string_length = cell_data.size();
            string whitespaces = string((column_widths.at(column) - string_length)/2, ' ');
            cout << "|";
            cout << whitespaces << cell_data << whitespaces;
            cout << ((2*whitespaces.size()+string_length) < column_widths.at(column) ? string(column_widths.at(column) - (2*whitespaces.size()+string_length), ' ') : "");
            cout << "|";
        }
        cout << endl;
        if(row+1 < total_rows)
            draw_horizontal_line(column_widths, '-');
    }
}
