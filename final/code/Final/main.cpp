#include "Idea_1.h"
#include "Idea_2.h"
int main(){
    int ch = 0;
    while (1)
    {
        cout << "Input 1 to use Idea_1,  Input 2 to use Idea_2,  Input anything else will end this program " << endl;
        cin >> ch;
        if (ch == 1)
        {
            Idea1();
            break;
        } 
        else if (ch == 2)
        {
            Idea2();
            break;
        }
        else
        {
            cout << "End" << endl;
            break;
        }
    }
    return 0;
}