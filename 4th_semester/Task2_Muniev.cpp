#include <iostream>

class List
{
private:
    class Node
    {
    public:
        Node *pNext, *pPrev;
        int data;
    };
    Node *head, *tail;
    int l_size; // размер списка
public:
    List();
    ~List();
    List(const List &Copy);
    List & operator = (const List & B);
    void push_front(int data);
    void push_back(int data);
    int front();
    int back();
    void pop_front();
    void pop_back();
    void insert(int p, int value);
    void erase(int p);
    bool empty();
    int size();
    void print();
};

List::List()
{
    head = NULL;
    tail = NULL;
    l_size = 0;
    std::cout << "Default constructor worked" << std::endl;
}

List::~List()
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

void List::push_back(int value)
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

void List::push_front(int value)
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

int List::front()
{
    return head->data;
}
int List::back()
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

void List::print()
{
    Node *temp = head;
    while (temp != NULL)
    {
        std::cout << temp->data << " ";
        temp = temp->pNext;
    }
    std::cout << std::endl;
}

void List::insert(int p, int value)
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
int main()
{
    int x;
    List l1;
    if (l1.empty()) std::cout << "l1 is empty" << std::endl;
    std::cout << "l1 size = " << l1.size() << std::endl;
    /* Удаление всех элементов с помощью pop_front() , pop_back(), erase(p) */
    List l4;
    l4.insert(0,666);
    l4.insert(0,777);
    l4.print();
    l4.pop_back();
    l4.pop_back();
    l4.push_front(88);
    l4.push_front(99);
    l4.print();
    l4.pop_front();
    l4.pop_front();
    l4.print();
    l4.push_front(555);
    l4.print();
    l4.erase(0);
    l4.print();
    // ----------------------------------------------------------
    std::cout << "l1.push_front(1) l1.push_front(2) l1.push_back(3) "
                 "l1.push_back(4) l1_push_front(5) l1.print() l.size() worked:" <<std::endl;
    l1.push_front(1);
    l1.push_front(2);
    l1.push_back(3);
    l1.push_back(4);
    l1.push_front(5);
    std::cout << "l1 size = " << l1.size() << std::endl;
    l1.print();
    List l2(l1);
    List l3;
    l3 = l1;
    l3.print();
    l2.print();
    x = l1.back();
    std::cout << "x = l1.back() worked, x = " << x << std::endl;
    x = l1.front();
    std::cout << "x = l1.front() worked, x = " << x << std::endl;

    /* было 5 2 1 3 4 */
    l1.insert(0, 22); // стало 22 5 2 1 3 4
    l1.insert(1, 33); // стало 22 33 5 2 1 3 4
    l1.insert(6,44); // вставляем перед перед 6ым элементом ( 4 ) 44 - 22 33 5 2 1 3 44 4
    l1.insert(7, 55); // перед 7ым ( 4 ) - 22 33 5 2 1 3 44 55 4
    l1.insert(100, 100); // некорректный ввод
    l1.print();
    std::cout << "l1 size = " << l1.size() << std::endl;
    // Оператор присваивания
    l3 = l1;
    l3.print();

    /* было 22 33 5 2 1 3 44 55 4 */
    l1.erase(0); // удалили нулевой элемент -> 33 5 2 1 3 44 55 4
    l1.erase(1); // удалили первый элемент -> 33 2 1 3 44 55 4
    l1.erase(7); // неверный ввод
    l1.erase(6); // удаляем шестой элемент -> 33 2 1 3 44 55
    l1.print(); // 33 2 1 3 44 55
    l2.print(); // 5 2 1 3 4
    l3.print(); // 22 33 5 2 1 3 44 55 4
    std::cout << "l1 size = " << l1.size() << std::endl;
    return 0;
}