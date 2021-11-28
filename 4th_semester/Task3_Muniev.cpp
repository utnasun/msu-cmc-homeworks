#include <iostream>
#include <cstring>
// Вариант 3
class AccountInfo
{
public:
    int accountID;
    char* accountOwner;
    char* accountDate;
    int accountMoney;
    AccountInfo();
    AccountInfo(int ID, char* Owner, char *Date, int Money);
    AccountInfo & operator= (const AccountInfo & B);
    AccountInfo (const AccountInfo &B);
    ~AccountInfo();
    void print() const;
};

AccountInfo::AccountInfo() // конструктор по умолчанию для элемента данных
{
    accountID = 0;
    accountOwner = new char[1];
    accountOwner[0] = '\0';
    accountDate = new char[1];
    accountDate[0] = '\0';
    accountMoney = 0;
}
AccountInfo::AccountInfo(int ID, char *Owner, char *Date, int Money) // конструктор для элемента данных
{
    accountID = ID;
    accountOwner = new char [strlen(Owner)+1];
    accountDate = new char [strlen(Date)+1];
    strcpy(accountOwner, Owner);
    strcpy(accountDate, Date);
    accountMoney = Money;
}
AccountInfo::AccountInfo(const AccountInfo & B) // конструктор копирования для элемента данных
{
    accountOwner = new char [strlen(B.accountOwner)+1];
    accountDate = new char [strlen(B.accountDate)+1];
    strcpy (accountOwner, B.accountOwner);
    strcpy (accountDate, B.accountDate);
    accountID = B.accountID;
    accountMoney = B.accountMoney;
}
AccountInfo& AccountInfo::operator=(const AccountInfo &B) // оператор = для элемента данных
{
    if (this == &B) return *this;
    delete[] accountDate;
    delete[] accountOwner;
    accountOwner = new char [strlen(B.accountOwner)+1];
    accountDate = new char [strlen(B.accountDate)+1];
    strcpy(accountOwner, B.accountOwner);
    strcpy(accountDate, B.accountDate);
    accountID = B.accountID;
    accountMoney = B.accountMoney;
    return *this;
}
AccountInfo::~AccountInfo() // деструктор элемента данных
{
    delete[] accountOwner;
    delete[] accountDate;
    //std::cout << "Account destructor worked" << std::endl;
}
void AccountInfo::print() const // печать элемента данных
{
    std::cout << "ID:" << accountID << " Name:" << accountOwner << " Date:" << accountDate
    << " Balance:" << accountMoney << std::endl;
}
// ------------------------------------------------------------------------------------------
class List
{
private:
    class Node
    {
    public:
        Node *pNext, *pPrev;
        AccountInfo data;
    };
    Node *head, *tail;
    int l_size; // размер списка
public:
    List();
    ~List();
    List(const List &Copy);
    List & operator = (const List & B);
    void push_front(AccountInfo data);
    void push_back(AccountInfo data);
    AccountInfo front();
    AccountInfo back();
    void pop_front();
    void pop_back();
    void insert(int p, AccountInfo value);
    void erase(int p);
    bool empty();
    int size();
    void print() const;
};

List::List()
{
    head = NULL;
    tail = NULL;
    l_size = 0;
    std::cout << "Default constructor worked" << std::endl;
}

List::~List() // Деструктор списка
{
    while (head!=NULL)
    {
        Node *temp = head;
        head = head->pNext;
        delete temp;
    }
    std::cout << "Destructor worked" << std::endl;
}

List::List(const List &Copy) // конструктор копирования списков
{
    this->head = NULL;
    this->tail = NULL;
    this->l_size = 0;
    Node *temp = Copy.head;
    for (int i=0; i<Copy.l_size; i++) // идем по копируемому списку и переносим из него данные в копирующий
    {
        push_back(temp->data);
        temp = temp->pNext;
    }
    std::cout << "Copy constructor worked" << std::endl;
}

List& List::operator=(const List &B)  // оператор присваивания списков
{
    if (this == &B) return *this;
    while (this->head != NULL) // удаляем текущий список
    {
        Node *temp = this->head;
        this->head = this->head->pNext;
        delete temp;
    }
    this->tail = NULL;
    this->l_size = 0;
    Node *temp = B.head;
    for (int i=0; i<B.l_size; i++) // переносим значения списка
    {
        push_back(temp->data);
        temp = temp->pNext;
    }
    std::cout << "Operator = worked" << std::endl;
    return *this;
}

void List::push_back(AccountInfo value)
{
    Node *temp = new Node;
    temp->data = value;
    temp->pNext = NULL;
    if (head != NULL) // если список не пустой
    {
        temp->pPrev = tail; // последний элемент в списке теперь указывает на предыдущий хвост
        tail->pNext = temp; // за старым хвостом следует добавленный элемент
        tail = temp; // новый элемент стал хвостом
    }
    else {
        temp->pPrev = NULL;
        head = temp;
        tail = temp;
    }
    l_size++;
}

void List::push_front(AccountInfo value)
{
    Node *temp = new Node;
    temp->data = value;
    temp->pPrev = NULL;
    if (head != NULL) // если список не пустой
    {
        temp->pNext = head; // первый элемент указывает на старую голову
        head->pPrev = temp; // prev головы указывает на предыдущий элемент
        head = temp; // новый элемент стал головой
    }
    else
    {
        temp->pNext = NULL;
        head = temp;
        tail = temp;
    }
    l_size++;
}

AccountInfo List::front()
{
    return head->data;
}
AccountInfo List::back()
{
    return tail->data;
}

void List::pop_front()
{
    if (head != NULL)
    {
        if (head != tail) {
            Node *temp = head;
            head = head->pNext; // меняем указатель головы на следующий элемент после нее
            head->pPrev = NULL;
            delete temp;
            l_size--;
        }
        else
        {
            Node *temp = head;
            head = NULL;
            delete temp;
            l_size--;
        }
    }
}

void List::pop_back()
{
    if (head != NULL)
    {
        if (tail != head) // если не последний
        {
            Node *temp = tail;
            tail = tail->pPrev; // меняем указатель хвоста на предпоследний элемент
            tail->pNext = NULL;
            delete temp; // удаляем предпоследний элемент
            l_size--;
        }
        else // если удаляем последний элемент
        {
            Node *temp = head;
            head = NULL;
            delete temp;
            l_size--;
        }

    }
}

void List::print() const
{
    Node *temp = head;
    while (temp != NULL)
    {
        temp->data.print();
        temp = temp->pNext;
    }
    std::cout << std::endl;
}

void List::insert(int p, AccountInfo value)
{
    /* Нумерация списка с 0 */
    if ( (p < 0) || ((p>=l_size) && (p != 0)) ) std::cout << "Incorrect position in insert(p,value), p = " << p << std::endl;
    /* Если позиция превышает количество элементов или отрицательная - ошибка */
    if (p==0)
    {
        push_front(value); // если перед 0-ым элементом, то просто вставляем в начало
    }

    if ((p > 0) && (p <= (l_size-1))){
        Node *after_insert = head;
        for (int i = 0; i<p; i++)
        {
            after_insert = after_insert->pNext; // доходим до элемента перед которым вставляем
        }
        Node *before_insert = after_insert->pPrev; // сохраняем указатель на элемент перед вставляемым
        Node *temp = new Node;
        temp->data = value;
        temp->pNext = after_insert; // в добавленном элементе устанавливаем указатели на элементы за ним и до него
        temp->pPrev = before_insert;
        after_insert->pPrev = temp; // устанавливаем указатели в элементах до и после вставленного элемента
        before_insert->pNext = temp;
        l_size++;
    }
}

void List::erase(int p)
{
    /* Нумерация списка с 0 */
    if ((p < 0) || (p >= l_size)) std::cout << "Incorrect position in erase(p), p = " << p << std::endl;

    if (p == 0)
    {
        pop_front(); // удаление 0 элемента = pop_front()
    }

    if ((p == (l_size-1)) && (p != 0))
    {
        pop_back(); // удаление элемента с позицией равной количеству элементов списка = pop_back()
    }

    if ((p > 0) && (p < (l_size-1)))
    {
        Node *erasing = head;
        for (int i = 0; i<p; i++)
        {
            erasing = erasing->pNext; // идем по списку до удаляемого элемента
        }
        Node *before = erasing->pPrev; // сохраняем указатели предыдущего и последующего за удаляемым
        Node *after = erasing->pNext;
        before->pNext = after; // устанавливаем указатели предыдущего и последующего за удаляемым друг на друга
        after->pPrev = before;
        delete erasing;
        l_size--;
    }
}

bool List::empty()
{
    return (head == NULL);
}

int List::size(){
    return l_size;
}

class Queue: private List // private-наследование, чтобы через объект очередь были доступны только его функции
{
    static int NumID; // нужна для единой нумерации счетов
public:
    int maxSize;
    Queue();
    Queue(int mSize);
    void back(AccountInfo value);
    AccountInfo front();
    void pop();
    bool empty();
    int size();
    bool full();
    void print() const;
};
Queue::Queue()
{
    maxSize = 100;
}
Queue::Queue(int mSize)
{
    maxSize = mSize;
}
void Queue::back(AccountInfo value)
{
    if (!full()) {
        NumID++;
        value.accountID = NumID; // нумерация счетов в очереди с 1
        List::push_back(value);
    }
    else std::cout << "Queue is full" << std::endl;

}
AccountInfo Queue::front()
{
    return (List::front());
}

void Queue::pop()
{
    List::pop_front();
}

bool Queue::empty()
{
    return (List::empty());

}

int Queue::size()
{
    return (List::size());
}

bool Queue::full()
{
    return (List::size() == maxSize);
}

void Queue::print() const
{
    List::print();
}

int Queue::NumID = 0;

int main()
{
    AccountInfo x1(1,"Utnasun Muniev","25.03.21",100)
    ,x2(-1,"X2","14.02.20",555),x3(-1,"X3","02.01.19",66),
    x4(-2,"X4","30.05.01",5000),x5(-2, "X5","25.03.05",30),
    x6(-3,"X6","2.02.19",888), x7(x1);
    List l1;
    Queue q1(5),q3(3);
    std::cout << "Queue q1, q1 is empty:" << q1.empty() << std::endl;
    q1.back(x1);
    q1.back(x2);
    q1.back(x3);
    q1.back(x4);
    q1.back(x5);
    std::cout << "q1.print() после добавления 5 элементов : " << std::endl;
    q1.print();
    q1.back(x6); // добавлю 6ой в элемент с очередью размером 5
                // Queue is full

    // Проверим единую нумерацию для всех очередей
    q3.back(x6);
    q3.back(x7);
    q3.back(x2);
    q3.print();
    // Проверка оператора присваивания очередей
    Queue q2;
    q2 = q1;
    std::cout << "q2 =q1, q2 maxSize:" <<q2.maxSize << std::endl;
    std::cout << "После q2 = q1, q2.print():" << std::endl;
    q2.print();
    std::cout << "q2 size:" << q1.size() << " q2 is full: " << q1.full() << " ,q2 is empty:" << q1.empty() << std::endl;
    std::cout << "q1 size:" << q1.size() << " q1 is full: " << q1.full() << " ,q1 is empty:" << q1.empty() << std::endl;

    // Проверяем работу q1.front()
    std::cout << std::endl << "x6.print(), x6=q1.front(), x6.print():" << std::endl;
    x6.print();
    x6 = q1.front();
    x6.print();

    // Проверяем работу q1.pop()
    q1.pop();
    // Теперь x2 - первый элемент, поэтому после x6 = q1.front() , x6=x2
    x6 = q1.front();
    x6.print();
    std::cout << std::endl << "q1.size() после q1.pop():" << q1.size() << std::endl;

    // Достаем все элементы из очереди
    q1.pop();
    q1.pop();
    q1.pop();
    q1.pop();
    std::cout << "q1.size() после удаления: " << q1.size() << ", q1 is empty:" << q1.empty() << ", q1 is full:"
    << q1.full() << std::endl;
    q1.print(); // печать пустой очереди

    // делаем лишний q1.pop()
    std::cout << "q2.size() после удаления всех элементов из q1: " << q2.size() << " , q2.print() :"<< std::endl;
    q1.pop();
    q2.print();
    std::cout <<"q1 size после лишнего q1.pop(): " << q1.size() << std::endl;


    // Работа со списком
    std::cout <<"Список l1: l1.push_back(x1),l1.push_back(x6), l1.print(): " << std::endl;
    l1.push_back(x1);
    l1.push_back(x6);
    l1.print();
    std::cout <<"Список l1: l1.pop_back, l1.print(), l1.pop_back, l1.print(): " << std::endl;
    l1.pop_back();
    l1.pop_back();
    l1.print();
    std::cout << "Список l1: l1.insert(0,x1), l1.print(), l1.erase(0), l1.print()" << std::endl;
    l1.insert(0,x1);
    l1.print();
    l1.erase(0);
    l1.print();
    return 0;
}