#include "main.h"

class imp_res : public Restaurant
{
private:
    // store restaurant customer list
    customer *head;
    customer *tail;
    customer *current;
    int noOfCustomerInRes;
    int numOfPositiveEnergyInRes;

    // store queue customer list
    customer *queueHead;
    customer *queueTail;
    int noOfCustomerInQueue;
    int numOfPositiveEnergyInQueue;

    // store customer order list in table
    customer *custormerOrderAllHead;
    customer *custormerOrderAllTail;

public:
    imp_res()
    {
        head = nullptr;
        tail = nullptr;
        current = nullptr;
        noOfCustomerInRes = 0;
        numOfPositiveEnergyInRes = 0;

        queueHead = nullptr;
        queueTail = nullptr;
        noOfCustomerInQueue = 0;
        numOfPositiveEnergyInQueue = 0;

        custormerOrderAllHead = nullptr;
        custormerOrderAllTail = nullptr;
    };

    ~imp_res()
    {
        customer *it = head;
        if (it)
        {
            do
            {
                customer *del = it;
                it = it->next;
                del->next = nullptr;
                del->prev = nullptr;
                delete del;
                del = nullptr;
            } while (it != head);
        }

        it = queueHead;
        while (it)
        {
            customer *del = it;
            it = it->next;
            del->next = nullptr;
            del->prev = nullptr;
            delete del;
            del = nullptr;
        }

        it = custormerOrderAllHead;
        while (it)
        {
            customer *del = it;
            it = it->next;
            del->next = nullptr;
            del->prev = nullptr;
            delete del;
            del = nullptr;
        }

        head = nullptr;
        tail = nullptr;
        current = nullptr;
        noOfCustomerInRes = 0;
        numOfPositiveEnergyInRes = 0;

        queueHead = nullptr;
        queueTail = nullptr;
        noOfCustomerInQueue = 0;
        numOfPositiveEnergyInQueue = 0;

        custormerOrderAllHead = nullptr;
        custormerOrderAllTail = nullptr;
    };

    /////////////////////////
    /* DEBUG FUNTIONS */
    // void printCustomer()
    // {
    //     cout << "In restaurant:\n";
    //     customer *it = current;
    //     if (it)
    //     {
    //         do
    //         {
    //             it->print();
    //             it = it->next;
    //         } while (it != current);
    //     }
    //     cout << "In queue:\n";
    //     it = queueHead;
    //     while (it)
    //     {
    //         it->print();
    //         it = it->next;
    //     }
    //     cout << "\n";
    // }

    // void printCustomerOrder()
    // {
    //     cout << "Customer order:\n";
    //     customer *it = custormerOrderAllHead;
    //     while (it)
    //     {
    //         it->print();
    //         it = it->next;
    //     }
    //     cout << "\n";
    // }

    // void checkList(customer *head)
    // {
    //     customer *it = head;
    //     if (it)
    //     {
    //         do
    //         {
    //             it->print();
    //             it = it->next;
    //         } while (it != head);
    //     }

    //     cout << "\n";
    // }
    /* END DEBUG FUNCTIONS */
    /////////////////////////

    /* SUPPORT FUNCTION */
    void customerSwap(customer *a, customer *b)
    {
        string tempName = a->name;
        int tempEnergy = a->energy;
        a->name = b->name;
        a->energy = b->energy;
        b->name = tempName;
        b->energy = tempEnergy;
    }

    // shellSortLL: Do shell sort on linked list from head to tail
    void shellSortLL(customer *head, customer *tail, int &counter)
    {
        int n = 0;
        customer *it = head;
        do
        {
            n++;
            it = it->next;
        } while (it && it != tail->next);

        for (int gap = n / 2; gap > 0; gap /= 2)
        {
            if(gap == 2) continue;
            customer *temp = head;
            customer *traverseFlag = head;
            while (true)
            {
                traverseFlag = temp;
                bool stop = false;
                for (int j = 0; j < gap; j++)
                {
                    if (!traverseFlag || traverseFlag == tail->next)
                    {
                        stop = true;
                        break;
                    }
                    traverseFlag = traverseFlag->next;
                }

                if (stop || traverseFlag == tail->next)
                    break;

                customer *temp2 = traverseFlag;
                if (temp2)
                    for (int j = 0; j < gap; j++)
                        temp2 = temp2->prev;
                customer *temp3 = traverseFlag;
                while (temp3 && temp2)
                {
                    int c = gap;
                    if (abs(temp3->energy) >= abs(temp2->energy))
                    {
                        if (abs(temp3->energy) > abs(temp2->energy))
                        {
                            customerSwap(temp3, temp2);
                            counter++;
                        }

                        else if (abs(temp3->energy) == abs(temp2->energy))
                        {
                            // if temp3 comes first in the customer order list, then swap
                            customer *checkOrder = custormerOrderAllHead;
                            while (checkOrder)
                            {
                                if (checkOrder->name == temp3->name)
                                {
                                    customerSwap(temp3, temp2);
                                    counter++;
                                    break;
                                }
                                else if (checkOrder->name == temp2->name)
                                    break;
                                checkOrder = checkOrder->next;
                            }
                        }
                    }
                    while (c-- && temp2)
                    {
                        temp2 = temp2->prev;
                    }
                    if (!temp2)
                        break;
                    c = gap;
                    while (c-- && temp3)
                        temp3 = temp3->prev;
                    if (!temp3)
                        break;
                }
                temp = temp->next;
            }
        }
    }

    void RED(string name, int energy)
    {
        /* CHECK ADDING VALIDITY */
        // energy = 0 then return
        if (energy == 0)
            return;

        // if name is already in the list, return
        customer *checkName = head;
        if (checkName)
        {
            do
            {
                if (checkName->name == name)
                    return;
                checkName = checkName->next;
            } while (checkName != head);
        }

        customer *checkNameQueue = queueHead;
        while (checkNameQueue)
        {
            if (checkNameQueue->name == name)
                return;
            checkNameQueue = checkNameQueue->next;
        }

        /* ADD TO CUSTOMER ORDER LIST */

        // if current size is >= size, add to queue
        bool addQueue = false;
        if (noOfCustomerInRes >= MAXSIZE)
        {
            // if queue current size is >= size, return
            if (noOfCustomerInQueue >= MAXSIZE)
                return;
            else
            {
                customer *cus = new customer(name, energy, nullptr, nullptr);
                if (noOfCustomerInQueue == 0)
                {
                    queueHead = cus;
                    queueTail = cus;
                }
                else
                {
                    queueTail->next = cus;
                    cus->prev = queueTail;
                    queueTail = cus;
                }
                noOfCustomerInQueue++;
                if (energy > 0)
                    numOfPositiveEnergyInQueue++;
                addQueue = true;
            }
        }

        // store customer name in the list
        if (custormerOrderAllHead == nullptr)
        {
            custormerOrderAllHead = new customer(name, energy, nullptr, nullptr);
            custormerOrderAllTail = custormerOrderAllHead;
        }
        else
        {
            custormerOrderAllTail->next = new customer(name, energy, custormerOrderAllTail, nullptr);
            custormerOrderAllTail = custormerOrderAllTail->next;
        }

        if (!addQueue)
        {

            /* ADD TO LIST */
            if (noOfCustomerInRes == 0)
            {
                customer *cus = new customer(name, energy, nullptr, nullptr);
                head = cus;
                tail = cus;
                current = cus;
                head->next = tail;
                tail->prev = head;
            }

            else
            {
                if (noOfCustomerInRes >= MAXSIZE / 2)
                {
                    int res = 0;
                    customer *it = current;   // iterator of customer list
                    customer *save = current; // save position to add later on
                    if (it)
                    {
                        do
                        {
                            // pairs of difference between energy and it->energy
                            if (abs(it->energy - energy) > abs(res))
                            {
                                res = energy - it->energy;
                                save = it;
                            }
                            it = it->next;
                        } while (it && it != current);
                    }

                    if (res > 0)
                    {
                        current = save; // let current point to the position to add
                        customer *addCus = new customer(name, energy, current, current->next);
                        if (current->next)
                            current->next->prev = addCus;
                        current->next = addCus;
                        if (current == tail)
                            tail = addCus;
                        current = addCus;
                    }

                    else
                    {
                        current = save; // let current point to the position to add
                        customer *addCus = new customer(name, energy, current->prev, current);
                        if (current->prev)
                            current->prev->next = addCus;
                        current->prev = addCus;
                        if (current == head)
                            head = addCus;
                        current = addCus;
                    }
                }

                else
                {
                    if (energy >= current->energy)
                    {
                        customer *addCus = new customer(name, energy, current, current->next);

                        if (current->next)
                            current->next->prev = addCus;
                        current->next = addCus;
                        if (current == tail)
                            tail = addCus;
                        current = addCus;
                    }

                    else
                    {
                        customer *addCus = new customer(name, energy, current->prev, current);
                        if (current->prev)
                            current->prev->next = addCus;
                        current->prev = addCus;

                        if (current == head)
                            head = addCus;
                        current = addCus;
                    }
                }
            }
            if (energy > 0)
                numOfPositiveEnergyInRes++;
            noOfCustomerInRes++;
        }
    }

    void BLUE(int num)
    {
        if (num == 0)
            return;
        if (num >= noOfCustomerInRes || num > MAXSIZE)
        {
            customer *it = head;
            if (it)
            { // cut doubly linked list
                if (tail->next)
                    tail->next->prev = nullptr;
                tail->next = nullptr;
                while (it)
                {
                    customer *del = it;
                    it = it->next;
                    if (del->next)
                        del->next->prev = nullptr;
                    del->next = nullptr;
                    string name = del->name;
                    delete del;
                    del = nullptr;

                    // also delete the name in the customer order list all
                    customer *delOrder = custormerOrderAllHead;
                    while (delOrder)
                    {
                        if (delOrder->name == name)
                        {
                            if (delOrder == custormerOrderAllHead)
                                custormerOrderAllHead = custormerOrderAllHead->next;
                            if (delOrder == custormerOrderAllTail)
                                custormerOrderAllTail = custormerOrderAllTail->prev;
                            if (delOrder->next != nullptr)
                                delOrder->next->prev = delOrder->prev;
                            if (delOrder->prev != nullptr)
                                delOrder->prev->next = delOrder->next;
                            delOrder->prev = nullptr;
                            delOrder->next = nullptr;
                            delete delOrder;
                            delOrder = nullptr;
                            break;
                        }
                        else
                            delOrder = delOrder->next;
                    }
                }
            }

            noOfCustomerInRes = 0;
            numOfPositiveEnergyInRes = 0;
            head = nullptr;
            tail = nullptr;
            current = nullptr;
            // now we have to add the customer from the queue to the restaurant, base on RED function
        }

        else
        {
            // we traverse the restaurant list to find the customer to kick, according to the customer order list
            customer *it = head;
            int ele = num;
            if (it)
            {
                customer *myTimeList = custormerOrderAllHead;
                while (ele && myTimeList)
                {
                    // first, we need to make sure that it->name does not appear in queue
                    bool found = false;
                    customer *checkQueue = queueHead;
                    while (checkQueue)
                    {
                        if (checkQueue->name == myTimeList->name)
                        {
                            found = true;
                            break;
                        }
                        checkQueue = checkQueue->next;
                    }

                    if (found)
                    {
                        myTimeList = myTimeList->next;
                        continue;
                    }

                    do
                    {
                        if (it->name == myTimeList->name)
                        {
                            if (it == head)
                                head = head->next;
                            else if (it == tail)
                                tail = tail->prev;
                            if (!head)
                            {
                                head = nullptr;
                                tail = nullptr;
                                current = nullptr;
                            }
                            customer *del = it;
                            // if (it->next != nullptr)
                            it->next->prev = it->prev;
                            // if (it->prev != nullptr)
                            it->prev->next = it->next;

                            current = it;
                            if (it->energy > 0)
                            {
                                numOfPositiveEnergyInRes--;
                                current = current->next;
                            }
                            else
                                current = current->prev;
                            noOfCustomerInRes--;
                            it = it->next;
                            del->prev = nullptr;
                            del->next = nullptr;
                            delete del;
                            del = nullptr;

                            // delete the name in the customer order list
                            customer *delOrder = myTimeList;
                            if (delOrder == custormerOrderAllHead)
                                custormerOrderAllHead = custormerOrderAllHead->next;
                            else if (delOrder == custormerOrderAllTail)
                                custormerOrderAllTail = custormerOrderAllTail->prev;
                            if (!custormerOrderAllHead)
                            {
                                custormerOrderAllHead = nullptr;
                                custormerOrderAllTail = nullptr;
                            }
                            if (delOrder->next != nullptr)
                                delOrder->next->prev = delOrder->prev;
                            if (delOrder->prev != nullptr)
                                delOrder->prev->next = delOrder->next;
                            myTimeList = myTimeList->next;
                            delOrder->prev = nullptr;
                            delOrder->next = nullptr;
                            delete delOrder;
                            delOrder = nullptr;
                            ele--;
                            break;
                        }
                        else
                            it = it->next;
                    } while (it && it != head);
                }
            }
        }

        while (queueHead && noOfCustomerInQueue > 0 && noOfCustomerInRes < MAXSIZE)
        {
            noOfCustomerInQueue--;
            if (queueHead->energy > 0)
                numOfPositiveEnergyInQueue--;
            customer *deleteHead = queueHead;
            string name = queueHead->name;
            int energy = deleteHead->energy;
            queueHead = queueHead->next;
            if (!queueHead)
            {
                queueHead = nullptr;
                queueTail = nullptr;
            }
            if (deleteHead->next)
                deleteHead->next->prev = nullptr;
            deleteHead->next = nullptr;
            // if current size is >= size, add to queue

            /* ADD TO LIST */
            if (noOfCustomerInRes == 0)
            {
                customer *cus = new customer(name, energy, nullptr, nullptr);
                head = cus;
                tail = cus;
                current = cus;
                head->next = tail;
                tail->prev = head;
            }

            else
            {
                if (noOfCustomerInRes >= MAXSIZE / 2)
                {
                    int res = 0;
                    customer *it = current;   // iterator of customer list
                    customer *save = current; // save position to add later on
                    if (it)
                    {
                        do
                        {
                            // pairs of difference between energy and it->energy
                            if (abs(it->energy - energy) > abs(res))
                            {
                                res = energy - it->energy;
                                save = it;
                            }
                            it = it->next;
                        } while (it && it != current);
                    }

                    if (res > 0)
                    {
                        current = save; // let current point to the position to add
                        customer *addCus = new customer(name, energy, current, current->next);
                        if (current->next)
                            current->next->prev = addCus;
                        current->next = addCus;
                        if (current == tail)
                            tail = addCus;
                        current = addCus;
                    }

                    else
                    {
                        current = save; // let current point to the position to add
                        customer *addCus = new customer(name, energy, current->prev, current);
                        if (current->prev)
                            current->prev->next = addCus;
                        current->prev = addCus;
                        if (current == head)
                            head = addCus;
                        current = addCus;
                    }
                }

                else
                {
                    if (energy >= current->energy)
                    {
                        customer *addCus = new customer(name, energy, current, current->next);

                        if (current->next)
                            current->next->prev = addCus;
                        current->next = addCus;
                        if (current == tail)
                            tail = addCus;
                        current = addCus;
                    }

                    else
                    {
                        customer *addCus = new customer(name, energy, current->prev, current);
                        if (current->prev)
                            current->prev->next = addCus;
                        current->prev = addCus;

                        if (current == head)
                            head = addCus;
                        current = addCus;
                    }
                }
            }
            if (energy > 0)
                numOfPositiveEnergyInRes++;
            noOfCustomerInRes++;
            // RED(deleteHead->name, deleteHead->energy);
            delete deleteHead;
            deleteHead = nullptr;
        }
    }

    void PURPLE()
    {
        // shell sort on the queue customer list
        // - find customer with maximum abs(energy). If multiple customers have the same abs(energy), choose the one with the latest.
        // - do shell sort on the customer list from head to that customer. If customers have the same abs(energy), the one comes first in the list will be
        // considered bigger (base on the customer order list)
        // - do BLUE <N mod MAXSIZE> with N is number of transitions between the customers in the queue

        // 1. find customer with maximum abs(energy). If multiple customers have the same abs(energy), choose the one with the latest.
        customer *it = queueHead;
        customer *max = it;

        while (it)
        {
            if (abs(it->energy) > abs(max->energy))
            {
                max = it;
            }
            else if (abs(it->energy) == abs(max->energy))
            {
                // if it comes latest in the customer order list, then choose it
                customer *checkOrder = custormerOrderAllTail;
                while (checkOrder)
                {
                    if (checkOrder->name == it->name)
                    {
                        max = it;
                        break;
                    }
                    else if (checkOrder->name == max->name)
                        break;
                    checkOrder = checkOrder->prev;
                }
            }
            it = it->next;
        }

        // 2. do shell sort on the customer list from head to pointer max. If customers have the same abs(energy), the one comes first in the list will be
        // considered bigger (base on the customer order list)
        int counter = 0;
        if (queueHead && max)
            shellSortLL(queueHead, max, counter);

        BLUE(counter % MAXSIZE);
    }

    void REVERSAL()
    {
        //  create 2 doubly linked list, one for storing positive enery customer and one for storing negative
        //  traverse the restaurant list, if the energy is positive, add to positive list, else add to negative list
        if (current)
        {
            customer *customerHead = current;
            customer *customerTail = current->next;
            // create "int" pointer
            int left = 0;
            int right = 1;
            string name = current->name;

            // positive enery
            while (-left + right <= noOfCustomerInRes)
            {
                while (customerHead->energy < 0 && -left + right <= noOfCustomerInRes)
                {
                    left--;
                    customerHead = customerHead->prev;
                }
                while (customerTail->energy < 0 && -left + right <= noOfCustomerInRes)
                {
                    right++;
                    customerTail = customerTail->next;
                }

                if (-left + right > noOfCustomerInRes)
                    break;

                // swap content
                customerSwap(customerHead, customerTail);
                customerHead = customerHead->prev;
                left--;
                customerTail = customerTail->next;
                right++;
            }

            customerHead = current;
            customerTail = current->next;
            left = 0;
            right = 1;

            // negative energy
            while (-left + right <= noOfCustomerInRes)
            {
                
                while (customerHead->energy > 0 && -left + right <= noOfCustomerInRes)
                {
                    left--;
                    customerHead = customerHead->prev;
                }
                while (customerTail->energy > 0 && -left + right <= noOfCustomerInRes)
                {
                    right++;
                    customerTail = customerTail->next;
                }

                if (-left + right > noOfCustomerInRes)
                    break;

                // swap content
                customerSwap(customerHead, customerTail);
                customerHead = customerHead->prev;
                left--;
                customerTail = customerTail->next;
                right++;
            }

            // find name and assign current back
            customer *it = head;
            if (it)
            {
                do
                {
                    if (it->name == name)
                    {
                        current = it;
                        break;
                    }
                    it = it->next;
                } while (it != head);
            }
        }
    }

    void UNLIMITED_VOID()
    {
        // start from current pointer, find the longest sublist of the customer list (number of elements >= 4), that has the sum of enery minimum
        // if there are multiple sublist with the same sum, choose the last-found sublist
        // then, print the customer in the list out clockwise, start from the minimum energy customer
        if (noOfCustomerInRes < 4)
            return;
        // 1. find sublist
        // strategy: Find on levels
        long long min = 1e9;
        int level = noOfCustomerInRes;
        customer *minHead = current;
        customer *saveHead = current;
        for (int i = noOfCustomerInRes; i >= 4; i--)
        {
            minHead = current;
            do
            {
                customer *subHead = minHead;
                int currentNumber = 0;
                long long currentSum = 0;
                while (currentNumber < i)
                {
                    currentSum += subHead->energy;
                    subHead = subHead->next;
                    currentNumber++;
                }

                if (currentSum <= min)
                {
                    if (currentSum < min)
                    {
                        min = currentSum;
                        level = i;
                        saveHead = minHead;
                    }

                    else if (currentSum == min && level == i)
                        saveHead = minHead;
                }
                minHead = minHead->next;
            } while (minHead != current);
        }

        // 2. print out the list clockwise, start from the minimum energy customer
        // find the minimum energy customer on saveHead
        customer *minCus = saveHead;
        // find index of minimum value of sublist, start of saveHead and iteration level
        int index = 0;
        int count = 0;
        int listMin = saveHead->energy;
        customer *readList = saveHead;
        do
        {
            if (readList->energy < listMin)
            {
                listMin = readList->energy;
                minCus = readList;
                index = count;
            }
            readList = readList->next;
            count++;
        } while (readList != saveHead && count <= level);

        //  left, right
        int left = index;
        int right = level - index;
        customer *savePos = minCus;
        // read to right;
        do
        {
            minCus->print();
            minCus = minCus->next;
            right--;
        } while (right > 0);

        // read from left
        if (savePos == saveHead)
            return;
        minCus = saveHead;
        do
        {
            minCus->print();
            minCus = minCus->next;
            left--;
        } while (left > 0);
        
    }

    void DOMAIN_EXPANSION()
    {
        // calculate the abs sum of all negative energy
        // if the sum is greater than the sum of all positive energy, then kick all positive energy
        // else kick all negative energy
        // do this with the customer resturant list and queue
        int sumNegative = 0;
        int sumPositive = 0;
        customer *it = head;
        if (it)
        {
            do
            {
                if (it->energy < 0)
                    sumNegative += it->energy;
                else
                    sumPositive += it->energy;
                it = it->next;
            } while (it != head);
        }
        it = queueHead;
        while (it)
        {
            if (it->energy < 0)
                sumNegative += it->energy;
            else
                sumPositive += it->energy;
            it = it->next;
        }

        if (abs(sumNegative) > sumPositive)
        {
            
            // first, we output the postitive energy customer, in the order of customerOrderList, from tail to head
            customer *traversal = custormerOrderAllTail;
            while(traversal){
                if(traversal->energy > 0){
                    traversal->print();
                }
                traversal = traversal->prev;
            }

            traversal = custormerOrderAllHead;
            while (traversal)
            {
                if (traversal->energy > 0)
                {
                    string name = traversal->name;

                    // 1. we find name in both waiting queue and restaurant list
                    customer *del = head;
                    bool found = false;
                    if (del)
                    {
                        do
                        {
                            if (del->name == name)
                            {
                                current = del->next;
                                if (del == head)
                                    head = head->next;
                                if (del == tail)
                                    tail = tail->prev;
                                if (!head)
                                {
                                    head = nullptr;
                                    tail = nullptr;
                                    current = nullptr;
                                }
                                // if (del->next != nullptr)
                                del->next->prev = del->prev;
                                // if (del->prev != nullptr)
                                del->prev->next = del->next;
                                del->prev = nullptr;
                                del->next = nullptr;
                                delete del;
                                del = nullptr;
                                numOfPositiveEnergyInRes--;
                                noOfCustomerInRes--;
                                found = true;
                                break;
                            }
                            del = del->next;
                        } while (del != head);
                    }

                    if (!found)
                    {
                        del = queueHead;
                        while (del)
                        {
                            if (del->name == name)
                            {
                                if (del == queueHead)
                                    queueHead = queueHead->next;
                                if (del == queueTail)
                                    queueTail = queueTail->prev;
                                if (del->next != nullptr)
                                    del->next->prev = del->prev;
                                if (del->prev != nullptr)
                                    del->prev->next = del->next;
                                del->prev = nullptr;
                                del->next = nullptr;
                                noOfCustomerInQueue--;
                                numOfPositiveEnergyInQueue--;
                                delete del;
                                del = nullptr;
                                break;
                            }
                            del = del->next;
                        }
                    }

                    // 2. delete this name from the overall time list
                    customer *delTraverse = traversal;
                    if (delTraverse == custormerOrderAllHead)
                        custormerOrderAllHead = custormerOrderAllHead->next;
                    else if (delTraverse == custormerOrderAllTail)
                        custormerOrderAllTail = custormerOrderAllTail->prev;
                    traversal = traversal->next;
                    if (delTraverse->next != nullptr)
                        delTraverse->next->prev = delTraverse->prev;
                    if (delTraverse->prev != nullptr)
                        delTraverse->prev->next = delTraverse->next;
                    delTraverse->prev = nullptr;
                    delTraverse->next = nullptr;
                    delete delTraverse;
                    delTraverse = nullptr;
                }
                else
                    traversal = traversal->next;
            }
        }

        else
        {
            customer *traversal = custormerOrderAllTail;
            while(traversal){
                if(traversal->energy < 0){
                    traversal->print();
                }
                traversal = traversal->prev;
            }
            
            traversal = custormerOrderAllHead;
            while (traversal)
            {
                if (traversal->energy < 0)
                {
                    string name = traversal->name;

                    // 1. we find name in both waiting queue and restaurant list
                    customer *del = head;
                    bool found = false;
                    if (del)
                    {
                        do
                        {
                            if (del->name == name)
                            {
                                current = del->prev;
                                if (del == head)
                                    head = head->next;
                                if (del == tail)
                                    tail = tail->prev;
                                if (!head)
                                {
                                    head = nullptr;
                                    tail = nullptr;
                                    current = nullptr;
                                }
                                // if (del->next != nullptr)
                                del->next->prev = del->prev;
                                // if (del->prev != nullptr)
                                del->prev->next = del->next;
                                del->prev = nullptr;
                                del->next = nullptr;
                                delete del;
                                del = nullptr;
                                noOfCustomerInRes--;
                                found = true;
                                break;
                            }
                            del = del->next;
                        } while (del != head);
                    }

                    if (!found)
                    {
                        del = queueHead;
                        while (del)
                        {
                            if (del->name == name)
                            {
                                if (del == queueHead)
                                    queueHead = queueHead->next;
                                if (del == queueTail)
                                    queueTail = queueTail->prev;
                                if (del->next != nullptr)
                                    del->next->prev = del->prev;
                                if (del->prev != nullptr)
                                    del->prev->next = del->next;
                                del->prev = nullptr;
                                del->next = nullptr;
                                noOfCustomerInQueue--;
                                delete del;
                                del = nullptr;
                                break;
                            }
                            del = del->next;
                        }
                    }

                    // 2. delete this name from the overall time list
                    customer *delTraverse = traversal;
                    if (delTraverse == custormerOrderAllHead)
                        custormerOrderAllHead = custormerOrderAllHead->next;
                    else if (delTraverse == custormerOrderAllTail)
                        custormerOrderAllTail = custormerOrderAllTail->prev;
                    traversal = traversal->next;
                    if (delTraverse->next != nullptr)
                        delTraverse->next->prev = delTraverse->prev;
                    if (delTraverse->prev != nullptr)
                        delTraverse->prev->next = delTraverse->next;
                    delTraverse->prev = nullptr;
                    delTraverse->next = nullptr;
                    delete delTraverse;
                    delTraverse = nullptr;
                }
                else
                    traversal = traversal->next;
            }
        }

        // add the customer from the queue to the restaurant, base on RED function
        while (queueHead && noOfCustomerInQueue > 0 && noOfCustomerInRes < MAXSIZE)
        {
            string name = queueHead->name;
            int energy = queueHead->energy;
            if(energy > 0)
                numOfPositiveEnergyInQueue--;
            noOfCustomerInQueue--;
            customer *deleteHead = queueHead;
            queueHead = queueHead->next;
            if (!queueHead)
            {
                queueHead = nullptr;
                queueTail = nullptr;
            }
            if (deleteHead->next)
                deleteHead->next->prev = nullptr;
            deleteHead->next = nullptr;
            if (noOfCustomerInRes == 0)
            {
                customer *cus = new customer(name, energy, nullptr, nullptr);
                head = cus;
                tail = cus;
                current = cus;
                head->next = tail;
                tail->prev = head;
            }

            else
            {
                if (noOfCustomerInRes >= MAXSIZE / 2)
                {
                    int res = 0;
                    customer *it = current;   // iterator of customer list
                    customer *save = current; // save position to add later on
                    if (it)
                    {
                        do
                        {
                            // pairs of difference between energy and it->energy
                            if (abs(it->energy - energy) > abs(res))
                            {
                                res = energy - it->energy;
                                save = it;
                            }
                            it = it->next;
                        } while (it && it != current);
                    }

                    if (res > 0)
                    {
                        current = save; // let current point to the position to add
                        customer *addCus = new customer(name, energy, current, current->next);
                        if (current->next)
                            current->next->prev = addCus;
                        current->next = addCus;
                        if (current == tail)
                            tail = addCus;
                        current = addCus;
                    }

                    else
                    {
                        current = save; // let current point to the position to add
                        customer *addCus = new customer(name, energy, current->prev, current);
                        if (current->prev)
                            current->prev->next = addCus;
                        current->prev = addCus;
                        if (current == head)
                            head = addCus;
                        current = addCus;
                    }
                }

                else
                {
                    if (energy >= current->energy)
                    {
                        customer *addCus = new customer(name, energy, current, current->next);

                        if (current->next)
                            current->next->prev = addCus;
                        current->next = addCus;
                        if (current == tail)
                            tail = addCus;
                        current = addCus;
                    }

                    else
                    {
                        customer *addCus = new customer(name, energy, current->prev, current);
                        if (current->prev)
                            current->prev->next = addCus;
                        current->prev = addCus;

                        if (current == head)
                            head = addCus;
                        current = addCus;
                    }
                }
            }
            if (energy > 0)
                numOfPositiveEnergyInRes++;
            noOfCustomerInRes++;

            delete deleteHead;
            deleteHead = nullptr;
        }
    }

    void LIGHT(int num)
    {
        if (num == 0)
        {
            customer *it = queueHead;
            while (it)
            {
                it->print();
                it = it->next;
            }
        }

        else if (num > 0)
        {
            customer *it = current;
            if (it)
            {
                do
                {
                    it->print();
                    it = it->next;
                } while (it && it != current);
            }
        }

        else
        {
            customer *it = current;
            if (it)
            {
                do
                {
                    it->print();
                    it = it->prev;
                } while (it && it != current);
            }
        }
    }
};