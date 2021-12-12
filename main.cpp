#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <sstream>
#include <windows.h>
using namespace std;



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
//arguments 1
//int pointer to account_type
void show_account_types_menu(string* account_type);

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

//declaring boolean type variable
//assigning false value by default
//this variable will be used as a flag
//to determine if a student or a teacher is logged in or not
bool USER_LOGGED_IN = false;

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

void sign_up()
{
    system("cls");

    string account_type;

    show_account_types_menu(&account_type);

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



    string file_name;

    if(account_type == "teacher")
        file_name = "teacher_credentials.txt";
    else if(account_type == "student")
        file_name = "student_credentials.txt";

    ifstream input_file(file_name);
    ofstream output_file(file_name, ios::app);





    enum TABLE_HEADERS
    {
        FIRST_NAME = 0,
        LAST_NAME = 1,
        USER_NAME = 2,
        PASSWORD = 3
    };

    bool user_exists = false;



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
                if(account_type == "teacher")
                    teachers_cred_vect.push_back(temp_vector);
                else if(account_type == "student")
                    students_cred_vect.push_back(temp_vector);
            }
            input_file.close();

    }

    string first_name, last_name, user_name, password;

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

        if(account_type == "teacher" && teachers_cred_vect.size() > 0)
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
        else if(account_type == "student" && students_cred_vect.size() > 0)
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
    if(account_type == "teacher" && teachers_cred_vect.size() > 0)
        output_file << endl;
    else if(account_type == "student" && students_cred_vect.size() > 0)
        output_file << endl;
    output_file << first_name << ","
                    << last_name << ","
                    << user_name << ","
                    << password;

    output_file.close();

    if(account_type == "teacher")
        teachers_cred_vect.clear();
    else if(account_type == "student")
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

    string account_type;



    show_account_types_menu(&account_type);



    string file_name;

    if(account_type == "teacher")
        file_name = "teacher_credentials.txt";
    else if(account_type == "student")
        file_name = "student_credentials.txt";

    ifstream input_file(file_name);


    enum TABLE_HEADERS
    {
        FIRST_NAME = 0,
        LAST_NAME = 1,
        USER_NAME = 2,
        PASSWORD = 3
    };

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
                if(account_type == "teacher")
                    teachers_cred_vect.push_back(temp_vector);
                else if(account_type == "student")
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

        if(account_type == "teacher" && teachers_cred_vect.size() > 0)
        {
            for(int i = 0; i < teachers_cred_vect.size(); i++)
            {
                if(teachers_cred_vect[i][TABLE_HEADERS::USER_NAME] == user_name && teachers_cred_vect[i][TABLE_HEADERS::PASSWORD] == password)
                {
                    USER_LOGGED_IN = true;
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
        else if(account_type == "student" && students_cred_vect.size() > 0)
        {
            for(int i = 0; i < students_cred_vect.size(); i++)
            {
                if(students_cred_vect[i][TABLE_HEADERS::USER_NAME] == user_name && students_cred_vect[i][TABLE_HEADERS::PASSWORD] == password)
                {
                    USER_LOGGED_IN = true;
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
            if(account_type == "teacher")
                teachers_cred_vect.clear();
            else if(account_type == "student")
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


void show_account_types_menu(string* account_type)
{
    enum choices
    {
        TEACHER = 1,
        STUDENT = 2,
    };

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
            case choices::TEACHER:
                *account_type = "teacher";
                break;
            case choices::STUDENT:
                *account_type = "student";
                break;
            default:
                get_danger_color();
                cout << "Input is out of range. Try again\t";
                continue;
        }
        break;
    }
}
