#include "main.h"

int ncr(int n, int r, int maxsize)
{
	if (r > n)
		return 0;

	long long int fact = 1;
	for (int i = 1; i <= r; i++)
	{
		fact *= n - i + 1;
		fact /= i;
	}

	return fact % maxsize;
}

// function to count the number of ways
// to rearrange the array to obtain same bst
long long countways(vector<int> &arr, int maxsize)
{
	// store the size of the array
	int n = arr.size();

	// base case
	if (n <= 2)
	{
		return 1;
	}

	// store the elements of the
	// left subtree of bst
	vector<int> leftsubtree;

	// store the elements of the
	// right subtree of bst
	vector<int> rightsubtree;

	// store the root node
	int root = arr[0];

	for (int i = 1; i < n; i++)
	{

		// push all the elements
		// of the left subtree
		if (arr[i] < root)
		{
			leftsubtree.push_back(arr[i]);
		}

		// push all the elements
		// of the right subtree
		else
		{
			rightsubtree.push_back(arr[i]);
		}
	}

	// store the size of leftsubtree
	int n1 = leftsubtree.size();

	// recurrence relation
	int countleft = countways(leftsubtree, maxsize) % maxsize;
	int countright = countways(rightsubtree, maxsize) % maxsize;

	return (ncr(n - 1, n1, maxsize) % maxsize) * countleft * countright;
}

// node structure for the huffman tree
struct huffmannode
{
	char originalchar;
	char caesarchar;
	int frequency;
	int seq;
	huffmannode *left;
	huffmannode *right;

	huffmannode(char ch, char caesar, int freq, int seq = 0) : originalchar(ch), caesarchar(caesar), frequency(freq), seq(seq), left(nullptr), right(nullptr) {}
};

// comparator for the priority queue, considers both frequency and original order
struct comparenodes
{
	bool operator()(huffmannode *const &n1, huffmannode *const &n2)
	{
		// if frequencies are equal, compare sequence numbers
		if (n1->frequency == n2->frequency)
		{
			return n1->seq > n2->seq;
		}
		return n1->frequency > n2->frequency;
	}
};

// huffman tree class
class huffman
{
public:
	huffman()
	{
		root = nullptr;
		rotateTime = 0;
	}

	void buildtree(const vector<pair<pair<char, char>, int>> &vec)
	{
		// build huffman tree with avl rotations (?) pending
		root = buildhuffmantreewithbalance(vec);
	}

	void printhuffman()
	{
		printhuffmancodes(root);
	}

	void findpathtocharacterhelper(huffmannode *node, char target, char origin, string currentpath, string &resultpath)
	{
		if (node == nullptr)
			return;

		if (node->caesarchar == target)
		{
			resultpath = currentpath;
			return;
		}

		findpathtocharacterhelper(node->left, target, origin, currentpath + "0", resultpath);
		findpathtocharacterhelper(node->right, target, origin, currentpath + "1", resultpath);
	}

	huffmannode *getroot() { return root; }

	// remove tree
	void removetreehelper(huffmannode **node)
	{
		if (!*node)
			return;
		removetreehelper(&(*node)->left);
		removetreehelper(&(*node)->right);
		delete *node;
		*node = nullptr;
	}

	void removetree()
	{
		removetreehelper(&root);
		root = nullptr;
	}

private:
	huffmannode *root;
	int rotateTime;

	int height(huffmannode *node)
	{
		if (!node)
			return 0;
		return 1 + max(height(node->left), height(node->right));
	};

	huffmannode *buildhuffmantreewithbalance(const vector<pair<pair<char, char>, int>> &vec)
	{
		priority_queue<huffmannode *, vector<huffmannode *>, comparenodes> pq;

		int seq = 0;
		// init seq number
		for (auto pair : vec)
		{
			pq.push(new huffmannode(pair.first.first, pair.first.second, pair.second, seq));
			seq++;
		}

		if (pq.size() == 1)
		{
			root = pq.top();
			pq.pop();
		}
		else
		{
			while (pq.size() != 1)
			{
				// print queue
				// cout << "\ncurrent queue: " << pq.size() << endl;
				// priority_queue<huffmannode *, vector<huffmannode *>, comparenodes> temp = pq;
				// while (!temp.empty())
				// {
				// 	cout << temp.top()->frequency << " " << temp.top()->originalchar << " " << temp.top()->caesarchar << "\n";
				// 	temp.pop();
				// }

				huffmannode *left = pq.top();
				pq.pop();

				huffmannode *right = pq.top();
				pq.pop();

				int sum = left->frequency + right->frequency;
				huffmannode *node = new huffmannode('_', '\0', sum, seq);
				seq++;
				node->left = left;
				node->right = right;
				// push the node back to the priority queue
				// of all nodes have the same frequency with the pushed node, the pushed node will be popped finally
				// pq.push(node);
				root = node;

				// balance?
				rotateTime = 3;
				root = balanceMyTree(root, rotateTime);

				// cout << "----------- current tree -----------\n";
				// printhuffman();
				// cout << "----------- ----------- -----------\n";

				root->seq = seq;

				pq.push(root);
			}
		}

		return root;
	}

	// function to perform an avl rotation (right rotation)
	huffmannode *rightrotate(huffmannode *y)
	{
		huffmannode *x = y->left;
		huffmannode *t2 = x->right;

		// perform rotation
		x->right = y;
		y->left = t2;

		return x;
	}

	// function to perform an avl rotation (left rotation)
	huffmannode *leftrotate(huffmannode *x)
	{
		huffmannode *y = x->right;
		huffmannode *t2 = y->left;

		// perform rotation
		y->left = x;
		x->right = t2;

		return y;
	}

	// get the balance factor for this node
	int balancefactor(huffmannode *node)
	{
		if (!node)
			return 0;
		return height(node->left) - height(node->right);
	};

	// function to balance the huffman tree using avl rotations
	huffmannode *balanceHuffmanTree(huffmannode *root, int &rotateTime, bool &rotated)
	{
		if (root == nullptr)
			return nullptr;

		if (rotateTime == 0 || rotated)
			return root;

		if (rotateTime > 0)
		{
			int balance = balancefactor(root);
			if (abs(balance) > 1)
			{
				if (balance > 1)
				{
					if (balancefactor(root->left) < 0)
						root->left = leftrotate(root->left);
					root = rightrotate(root);
				}
				else if (balance < -1)
				{
					if (balancefactor(root->right) > 0)
						root->right = rightrotate(root->right);
					root = leftrotate(root);
				}

				rotateTime -= 1;
				rotated = true;
			}
		}

		// Balance the left subtree
		root->left = balanceHuffmanTree(root->left, rotateTime, rotated);

		// Balance the right subtree
		root->right = balanceHuffmanTree(root->right, rotateTime, rotated);

		return root;
	}

	huffmannode* balanceMyTree(huffmannode* root, int &rotateTime) {
		bool rotated = false;
		while (1) {
			root = balanceHuffmanTree(root, rotateTime, rotated);
			if (!rotated || rotateTime==0) break;
			else rotated = false;
		}

		return root;
	}

	void printhuffmancodes(huffmannode *root)
	{
		// lnr
		if (!root)
			return;
		printhuffmancodes(root->left);
		if (root->caesarchar != '\0')
			cout << root->caesarchar << endl;
		else
			cout << root->frequency << endl;
		printhuffmancodes(root->right);
	}
};

struct customer
{
	int resultdecimal;
	customer *left;
	customer *right;
	int area_id;

	~customer()
	{
		delete left;
		delete right;
		left = nullptr;
		right = nullptr;
	}

	customer()
	{
		resultdecimal = 0;
		left = nullptr;
		right = nullptr;
		area_id = 0;
	}

	customer(int res, int id, customer *left = nullptr, customer *right = nullptr)
	{
		this->resultdecimal = res;
		this->left = left;
		this->right = right;
		this->area_id = id;
	}
};

class bst_g
{
	queue<customer> customers;
	customer *root;

public:
	bst_g()
	{
		root = nullptr;
	}

	customer *getroot() { return root; }
	queue<customer> getcustomers() { return customers; }

	void remove_bst(customer **customer)
	{
		if (!*customer)
			return;
		remove_bst(&(*customer)->left);
		remove_bst(&(*customer)->right);
		delete *customer;
		*customer = nullptr;
	}

	void removebst()
	{
		// pass root as reference to remove_bst
		// cout << "before removing bst: " << root << endl;
		remove_bst(&root);
		// cout << "after removing bst: " << root << endl;
		//  root = nullptr;
		// dequeue all customer
		while (!customers.empty())
		{
			customers.pop();
		}
	}

	void insert(customer Customer)
	{
		if (root == nullptr)
			root = new customer(Customer.resultdecimal, Customer.area_id);

		else
		{
			customer *current = root;
			while (current != nullptr)
			{
				if (Customer.resultdecimal < current->resultdecimal)
				{
					if (current->left == nullptr)
					{
						current->left = new customer(Customer.resultdecimal, Customer.area_id);
						break;
					}
					current = current->left;
				}

				else
				{
					if (current->right == nullptr)
					{
						current->right = new customer(Customer.resultdecimal, Customer.area_id);
						break;
					}
					current = current->right;
				}
			}
		}

		customers.push(customer(Customer.resultdecimal, Customer.area_id));
	}

	// use minimum value of right subtree to replace the node to be deleted
	void removecustomer(int result)
	{
		// find the node to be deleted
		customer *current = root;
		customer *parent = nullptr;
		while (current != nullptr)
		{
			if (current->resultdecimal == result)
				break;

			parent = current;
			if (result < current->resultdecimal)
				current = current->left;
			else
				current = current->right;
		}

		// if the node to be deleted is not found
		if (current == nullptr)
			return;

		// if the node to be deleted is a leaf node
		if (current->left == nullptr && current->right == nullptr)
		{
			if (current != root)
			{
				if (parent->left == current)
					parent->left = nullptr;
				else
					parent->right = nullptr;
			}

			else
				root = nullptr;

			delete current;
			current = nullptr;
		}

		// if the node to be deleted has only one child
		else if (current->left && current->right == nullptr)
		{
			if (current != root)
			{
				if (parent->left == current)
				{
					parent->left = current->left;
					current->left = nullptr;
				}
				else
				{
					parent->right = current->left;
					current->left = nullptr;
				}
			}

			else
			{
				root = current->left;
				current->left = nullptr;
			}

			delete current;
			current = nullptr;
		}

		else if (current->left == nullptr && current->right)
		{
			if (current != root)
			{
				if (parent->left == current)
				{
					parent->left = current->right;
					current->right = nullptr;
				}
				else
				{
					parent->right = current->right;
					current->right = nullptr;
				}
			}

			else
			{
				root = current->right;
				current->right = nullptr;
			}

			delete current;
			current = nullptr;
		}

		// if the node to be deleted has two children
		else if (current->left && current->right)
		{
			customer *successor = current->right;
			customer *presuccess = current;
			while (successor->left != nullptr)
			{
				presuccess = successor;
				successor = successor->left;
			}

			// current->name = successor->name;
			current->resultdecimal = successor->resultdecimal;

			if (presuccess == current)
			{
				presuccess->right = successor->right;
				successor->right = nullptr;
			}
			else
			{
				presuccess->left = successor->right;
				successor->right = nullptr;
			}

			delete successor;
			successor = nullptr;
		}

		// pop from queue
		customers.pop();
	}

	// in-order traversal, return a vector
	void preordertraversal(customer *node, vector<int> &vec)
	{
		// base case
		if (node == nullptr)
			return;

		vec.push_back(node->resultdecimal);
		preordertraversal(node->left, vec);
		preordertraversal(node->right, vec);
	}

	// in-order traversal, just print
	void inorderhelp(customer *node)
	{
		// base case
		if (node == nullptr)
			return;

		inorderhelp(node->left);
		cout << node->resultdecimal << endl;
		inorderhelp(node->right);
	}

	void inorder()
	{
		inorderhelp(root);
	}

	void printQueue()
	{
		cout << "-----------------------------\n";
		queue<customer> temp = customers;
		while (!temp.empty())
		{
			cout << temp.front().resultdecimal << endl;
			temp.pop();
		}
		cout << "-----------------------------\n";
	}
};

struct area_s
{
	// int heap_id; // index in the heap
	vector<customer> customers;
	int priority;

	area_s()
	{
		customers = {};
		priority = -1;
		// heap_id = -1;
	}
};

int global_priority = 0;

class minheap_s
{
	int maxsize; // maxsize
	area_s *areas;
	// heap only
	vector<pair<pair<int, int>, int>> heap_area;

public:
	// heapify a subtree with root at given index
	// phần tử con sẽ hoán đổi vị trí với phần tử cha của nó khi giá trị num của phần tử con nhỏ hơn so với phần tử cha của nó
	// khi áp dụng re-heap down (di chuyển một phần tử xuống node lá), phần tử cha sẽ hoán đổi với phần tử con của nó khi giá trị num của phần tử cha lớn hơn
	// trường hợp phần tử cha có hai con, thì phần tử cha sẽ hoán đổi với phần tử con có giá trị lớn hơn.
	// nếu các phần tử có cùng giá trị num thì phần tử lớn hơn được quy ước là phần tử được thêm vào heap sớm hơn.
	minheap_s()
	{
		maxsize = 0;
		areas = nullptr;
		heap_area = {};
	}

	minheap_s(int maxsize)
	{
		this->maxsize = maxsize;
		// this->areas.resize(maxsize);
		areas = new area_s[maxsize];
		for (int i = 0; i < maxsize; i++)
		{
			areas[i] = area_s();
		}
		heap_area = {};
	}

	area_s *getareas() { return areas; }
	int getmaxsize() { return maxsize; }
	vector<pair<pair<int, int>, int>> &getheaparea() { return heap_area; }

	void killArea()
	{
		delete[] areas;
		areas = nullptr;
	}

	// first.first element is size
	// first.second element is index in the areas
	// second element is priority

	void heapify(int index)
	{
		int left = 2 * index + 1;
		int right = 2 * index + 2;

		// key is size of area
		int smallest = index;
		int s = heap_area.size();

		if (left < s && heap_area[left].first.first < heap_area[smallest].first.first)
			smallest = left;

		if (right < s && heap_area[right].first.first < heap_area[smallest].first.first)
			smallest = right;

		// check if the priorities are the same, and update 'smallest' based on priority
		if (left < s && heap_area[left].first.first == heap_area[smallest].first.first &&
			heap_area[left].second < heap_area[smallest].second)
			smallest = left;

		if (right < s && heap_area[right].first.first == heap_area[smallest].first.first &&
			heap_area[right].second < heap_area[smallest].second)
			smallest = right;

		if (smallest != index)
		{
			swap(heap_area[index], heap_area[smallest]);
			heapify(smallest); // recursively apply heapify
		}
	}

	// insert a customer into the heap, specifying the id of the area
	void insert(customer Customer, int id)
	{
		// customer->printinfo();
		// copy customer
		customer newcustomer = customer(Customer.resultdecimal, Customer.area_id);
		areas[id].customers.push_back(newcustomer);
		// update priority, if priority = -1
		// if (areas[id].priority == -1)
		//{
		areas[id].priority = global_priority;
		global_priority++;
		//}

		// filter out areas that have customers.size() > 0
		// heap_area.clear();
		bool pushBack = true;
		int s = heap_area.size();
		for (int i = 0; i < s; i++)
		{
			if (heap_area[i].first.second == id)
			{
				pushBack = false;
				heap_area[i].first.first = areas[id].customers.size();
				heap_area[i].second = areas[id].priority;
				break;
			}
		}

		// for (int i = 0; i < maxsize; i++)
		// {
		// 	cout << "Area " << i << " has " << areas[i].customers.size() << " customers, priority: " << areas[i].priority << endl;
		// }

		if (pushBack)
		{
			heap_area.push_back(pair<pair<int, int>, int>(pair<int, int>(areas[id].customers.size(), id), areas[id].priority));
			// areas[id].heap_id = heap_area.size() - 1;
		}

		int leaf_idx = heap_area.size() / 2 - 1;
		for (int i = leaf_idx; i >= 0; i--)
			heapify(i);
	}
};

// 2 restaurants: g and s
// each restaurant has maxsize areas

class g
{
	int maxsize; // maxsize
	// vectors of areas
	// vector<bst_g> areas;
	bst_g *areas;
	vector<customer> customers_g;

public:
	g()
	{
		maxsize = 0;
		areas = nullptr;
		customers_g = {};
	}

	g(int maxsize)
	{
		this->maxsize = maxsize;
		// this->areas.resize(maxsize);
		// this->customers_g.resize(0);
		areas = new bst_g[maxsize];

		for (int i = 0; i < maxsize; i++)
		{
			areas[i] = bst_g();
		}
	}

	int getmaxsize() { return maxsize; }
	bst_g *getareas() { return areas; }
	vector<customer> getcustomers() { return customers_g; }

	void killArea()
	{
		delete[] areas;
		areas = nullptr;
	}

	void addcustomer(customer Customer, int id)
	{
		// add customer
		// areas[id].insert(customer);
		// copy customer
		customer newcustomer = customer(Customer.resultdecimal, Customer.area_id);
		areas[id].insert(newcustomer);
		// areas[id].removebst();
		// areas[id].inorder();
		this->customers_g.emplace_back(newcustomer);
	}

	// debug
	void printcustomers()
	{
		cout << "---------------- RESTAURANT G: ----------------\n";
		int s = customers_g.size();
		for (int i = 0; i < s; i++)
			cout << customers_g[i].resultdecimal << " " << customers_g[i].area_id << endl;
		cout << "---------------- ------------- ----------------\n";
	}
};

class s
{
	int maxsize; // maxsize
	// each area is a node in a min-heap
	minheap_s min_heap;
	vector<customer> customers_s; // fifo order list

public:
	s() : maxsize(0) {}
	minheap_s &getminheap() { return min_heap; }
	vector<customer> getcustomers() { return customers_s; }
	int getmaxsize() { return maxsize; }

	s(int maxsize)
	{
		this->maxsize = maxsize;
		min_heap = minheap_s(maxsize);
		// min_heap.getareas().resize(maxsize);
		customers_s = {};
		for (int i = 0; i < maxsize; i++)
		{
			min_heap.getareas()[i] = area_s();
		}
	}

	void addcustomer(customer customer, int id)
	{
		// add customer with min_heap
		min_heap.insert(customer, id);
		// customer newcustomer = customer(customer.name, customer.vecmap, customer.resultdecimal);
		this->customers_s.push_back(customer);
	}

	// debug
	void printcustomers()
	{
		cout << "---------------- RESTAURANT S: ----------------\n";
		int s = customers_s.size();
		for (int i = 0; i < s; i++)
			cout << customers_s[i].resultdecimal << " " << customers_s[i].area_id << endl;
		cout << "---------------- ------------- ----------------\n";
	}

	void removecustomer(const customer &customertoremove)
	{
		customers_s.erase(remove_if(customers_s.begin(), customers_s.end(),
									[&customertoremove](const customer &customer)
									{
										return customer.resultdecimal == customertoremove.resultdecimal;
									}),
						  customers_s.end());
	}
};

class command
{
	int maxsize;   // maxsize
	g restaurantg; // restaurant g
	s restaurants; // restaurant s
	string last_customer;

public:
	command()
	{
		maxsize = 0;
		restaurantg = g();
		restaurants = s();
		last_customer = "";
	}

	void setmaxsize(int size)
	{
		maxsize = size;
		restaurantg = g(maxsize);
		restaurants = s(maxsize);
	}

	g getrestaurantg() { return restaurantg; }
	s getrestaurants() { return restaurants; }
	int getmaxsize() { return maxsize; }

	// handle lapse
	void lapse(string name)
	{
		// 1. chuẩn hóa
		/*
			đầu tiên, nhân viên sẽ liệt kê tần suất xuất hiện của từng ký tự riêng biệt trong tên của
			khách hàng (phân biệt chữ hoa và chữ thường). sau đó sắp xếp lại chúng theo thứ tự
			tăng dần (nếu có nhiều ký tự với tần suất xuất hiện như nhau thì ký tự nào xuất hiện
			trước sẽ đứng trước).
			ví dụ: với khách hàng có tên "abaaabbbdd" thì sau khi xử lý sẽ được danh sách:
			a : 4 −→ b : 4 −→ d : 1 −→ d : 1.
		*/

		map<char, int> freq;
		for (char c : name)
			freq[c]++;

		// if no at least 3 char then dont accept
		if (freq.size() < 3)
			return;

		vector<pair<pair<char, char>, int>> vec;
		// <<original char,caesar char>, frequency>

		for (auto &entry : freq)
			vec.push_back({{entry.first, entry.first}, entry.second});

		// for (auto &entry : vec)
		// {
		// 	cout << entry.first.first << ": " << entry.second << endl;
		// }

		for (auto &entry : vec)
		{
			int shift = entry.second;
			int multiplier = static_cast<int>(entry.first.first) / 32;
			int remainder = static_cast<int>(entry.first.first) % 32;

			// entry.first.second = 'a' + (entry.first.first - 'a' + shift) % 26;
			entry.first.second = static_cast<char>(multiplier * 32 + 1 + (remainder - 1 + shift) % 26);
		}

		// cout << "------------" << endl;
		// for (auto &entry : vec)
		// {
		// 	cout << entry.first.second << ": " << entry.second << endl;
		// }
		// cout << "------------" << endl;

		// sort vec with respect to caesar char
		sort(vec.begin(), vec.end(),
			 [](const auto &lhs, const auto &rhs)
			 {
				 return lhs.first.second < rhs.first.second;
			 });

		// cout << "------------" << endl;
		// for (auto &entry : vec)
		// {
		// 	cout << entry.first.second << ": " << entry.second << endl;
		// }

		// combine same caesar char
		int vecsize = vec.size();
		for (int i = 0; i < vecsize - 1; i++)
		{

			if (vec[i].first.second == vec[i + 1].first.second)
			{
				vec[i].second += vec[i + 1].second;
				vec.erase(vec.begin() + i + 1);
				i--;
			}
			vecsize = vec.size();
		}

		// cout << "------------" << endl;
		// for (auto &entry : vec)
		// {
		// 	cout << entry.first.second << ": " << entry.second << endl;
		// }

		// sort with frequency from lower to higher
		// if same frequency, sort with caesar char such that: lowercase before uppercase, and alphabetically from a to z, a to z
		sort(vec.begin(), vec.end(),
			 [](const auto &lhs, const auto &rhs)
			 {
				 if (lhs.second != rhs.second)
				 {
					 return lhs.second < rhs.second;
				 }
				 // in the group of same frequency --> lower case to upper case
				 if (islower(lhs.first.second) && isupper(rhs.first.second))
					 return true;
				 else if (isupper(lhs.first.second) && islower(rhs.first.second))
					 return false;

				 // in the group of all lower case and upper case --> alphabetically
				 return lhs.first.second < rhs.first.second;
			 });

		// print map
		// cout << "------------" << endl;
		// for (auto &entry : vec)
		// {
		// 	cout << entry.first.second << ": " << entry.second << endl;
		// }
		// cout << "------------" << endl;

		// 3. huffman
		huffman myhuffman;
		myhuffman.buildtree(vec);

		// cout << "SIZE OF VEC: " << vec.size() << "\n";

		if (myhuffman.getroot()->caesarchar != '\0' && vec.size() > 1)
		{
			// cout << "huffman tree is empty" << endl;
			myhuffman.removetree();
			return;
		}

		last_customer = name;
		// myhuffman.printhuffman();
		/*
			dựa vào cây myhuffman, thực hiện chuyển đổi từng ký tự trong tên của khách hàng (đã được
			chuyển đổi theo mã hóa ceasar) theo cách tương tự như mô tả trong tài liệu hướng
			dẫn [4] ở chương 5.6.2, thu được một giá trị theo hệ nhị phân. lấy tối đa 10 ký tự trong
			chuỗi nhị phân đó theo chiều từ phải sang trái và chuyển đổi kết quả thu được sang hệ
			thập phân. gọi kết quả thu được là result.
		*/

		string result = "";
		int size = name.length();
		for (int i = 0; i < size; i++)
		{
			// caesar
			int shift = freq[name[i]];
			int multiplier = static_cast<int>(name[i]) / 32;
			int remainder = static_cast<int>(name[i]) % 32;
			char temp = static_cast<char>(multiplier * 32 + 1 + (remainder - 1 + shift) % 26);

			string path = "";
			myhuffman.findpathtocharacterhelper(myhuffman.getroot(), temp, i, "", path);
			// cout << temp << " " << path << endl;
			result += path;
		}

		// free myhuffman
		myhuffman.removetree();

		// take last 10 bits of result
		if (result.length() > 10)
			result = result.substr(result.length() - 10, 10);

		reverse(result.begin(), result.end());

		// cout << result << "\n";

		// change to decimal form
		int decimal = 0;
		int l = result.length();
		for (int i = 0; i < l; i++)
			decimal += (result[i] - '0') * pow(2, result.length() - i - 1);

		// customer *newcustomer = new customer(name, vec, decimal);

		// myhuffman.printhuffman();

		/*
			nếu giá trị result là một giá trị lẽ thì khách sẽ thích ngồi ở nhà g, ngược lại, khách hàng
			sẽ ngồi ở nhà s. mỗi nhà hàng đều sẽ có maxsize khu vực.
			sau khi chọn được nhà hàng thì khách hàng sẽ được nhân viên sắp xếp vào các khu vực nhất
			định. số thứ tự khu vực (id) sẽ được tính theo công thức id = result % maxsize +
			1 với id bắt đầu từ 1 đến maxsize.
		*/
		int id = decimal % maxsize + 1;

		// cout << result << "-" << decimal << "-" << id << endl;
		if (decimal % 2 == 0)
		{ // s
			restaurants.addcustomer(customer(decimal, id - 1), id - 1);
			// cout << "s:\n";
			// restaurants.printcustomers();
			// cout << "-----------------------------------" << endl;
		}
		else
		{ // g
			restaurantg.addcustomer(customer(decimal, id - 1), id - 1);
			// cout << "g:\n";
			// restaurantg.printcustomers();
			// cout << "-----------------------------------" << endl;
		}
	}

	void kokusen()
	{

		// restaurantg.printcustomers();
		// apply for g restaurant
		for (int i = 0; i < restaurantg.getmaxsize(); i++)
		{
			// get the area with the most customers
			vector<int> vec{};
			restaurantg.getareas()[i].preordertraversal(restaurantg.getareas()[i].getroot(), vec);
			// pascal triangle
			// fillpascaltriangle(vec.size());
			// count permutations
			// int res = countpermutations(vec);
			// long long fact[vec.size()];
			// calculatefact(fact, vec.size(), maxsize);
			long long res = countways(vec, maxsize);

			res = res % maxsize;
			// kick res customers out of the area, fifo order
			int count = 0;
			int maxi = restaurantg.getareas()[i].getcustomers().size();
			if (res > maxi)
			{
				// delete the bst
				restaurantg.getareas()[i].removebst();
				// reset bst
				// restaurantg.getareas()[i].resetroot();
			}

			else
			{
				while (count < res)
				{
					// find
					// cout << "BEFORE REMOVE:\n";
					// restaurantg.getareas()[i].printQueue();
					customer popcustomer = restaurantg.getareas()[i].getcustomers().front();
					// kick info
					// cout << popcustomer.resultdecimal << "-" << i + 1 << endl;
					// find the customer in the bst
					restaurantg.getareas()[i].removecustomer(popcustomer.resultdecimal);
					// remove the customer from the queue
					// restaurantg.getareas()[i].getcustomers().pop();
					// cout << "AFTER REMOVE:\n";
					// restaurantg.getareas()[i].printQueue();
					count++;
				}
			}
		}
	}

	// apply for restaurant s
	void keiteiken(int num)
	{
		// print areas
		// for (int i = 0; i < restaurants.getminheap().getmaxsize(); i++)
		// {
		// 	cout << "area " << i << " has id " << restaurants.getminheap().getareas()[i].heap_id << endl;
		// }
		vector<pair<pair<int, int>, int>> areas = restaurants.getminheap().getheaparea();
		int s = areas.size();

		sort(areas.begin(), areas.end(),
			 [](const auto &lhs, const auto &rhs)
			 {
				 // sort areas based on size of area from smallest to largest
				 if (lhs.first.first != rhs.first.first)
				 {
					 return lhs.first.first < rhs.first.first;
				 }

				 else
					 return lhs.second < rhs.second;
			 });

		// print areas
		int deltime = 0;
		for (int i = 0; i < s && deltime < num; i++)
		{
			// cout << "---------AREAS IN HAND -------\n";
			// cout << i << " " << s << "\n";
			// for (int j= 0; j < areas.size(); j++)
			//{
			//	cout << areas[j].first.first << " " << areas[j].first.second << " " << areas[j].second << endl;
			//}
			// cout << "----------------------\n";
			// //cout << s << " " << num << endl;
			// cout << "i: " << i << "\n";
			// // kick num customers out of the area, fifo order
			// cout << "INDEX in area: " << areas[i].first.second << "\n";
			int count = 0;
			// restaurants.getminheap().printheap();
			int maximum = restaurants.getminheap().getareas()[areas[i].first.second].customers.size();
			while (count < min(maximum, num))
			{
				areas[i].first.first--;

				// front customer
				customer popcustomer = restaurants.getminheap().getareas()[areas[i].first.second].customers.front();
				// pop
				restaurants.getminheap().getareas()[areas[i].first.second].customers.erase(restaurants.getminheap().getareas()[areas[i].first.second].customers.begin());

				// cout << "------------ AREAS ----------------\n";
				// for (int j = 0; j < areas.size(); j++)
				// {
				// 	cout << "SIZE: " << areas[j].first.first << " INDEX: " << areas[j].first.second << " PRIORITY: " << areas[j].second << endl;
				// }
				// cout << "------------ ---- ----------------\n";
				// cout << "--------- BEFORE DELETE --------------\n";
				// for (int j = 0; j < restaurants.getminheap().getheaparea().size(); j++)
				// {
				// 	cout << "size: " << restaurants.getminheap().getheaparea()[j].first.first << " index " << restaurants.getminheap().getheaparea()[j].first.second << " priority " << restaurants.getminheap().getheaparea()[j].second << endl;
				// }
				// cout << "--------------------------------------\n";

				// cout << restaurants.getminheap().getareas()[areas[i].first].customers.size() << endl;
				// update in heap
				// restaurants.getminheap().getheaparea()[restaurants.getminheap().getareas()[areas[i].first].heap_id].first.first--;
				int si = restaurants.getminheap().getheaparea().size();
				for (int j = 0; j < si; j++)
				{
					if (areas[i].first.second == restaurants.getminheap().getheaparea()[j].first.second)
					{
						restaurants.getminheap().getheaparea()[j].first.first--;
						// update priorities
						restaurants.getminheap().getheaparea()[j].second = global_priority;
						areas[i].second = global_priority;
						global_priority++;
						break;
					}
				}
				// cout << restaurants.getminheap().getheaparea()[areas[i].first].first.first << " " << restaurants.getminheap().getheaparea()[areas[i].first].first.second << " " << restaurants.getminheap().getheaparea()[areas[i].first].second << endl;
				cout << popcustomer.resultdecimal << "-" << areas[i].first.second + 1 << endl;

				// if size of the area is 0, erase the area
				if (restaurants.getminheap().getareas()[areas[i].first.second].customers.size() == 0)
				{
					// cout << "--------------------------------------\n";
					// swap this area with the final area
					// cout << "SWAP AREA NUMBER: " << areas[i].first.second << "\n";
					// cout << "CHECK HEAD: " << areas[0].first.second << "\n";
					// cout << "CHECK TAIL: " << areas[s - 1].first.second << "\n";
					// cout << "INTEND TO SWAP: " << restaurants.getminheap().getheaparea()[i].first.second << "\n";
					// cout << "TAIL: " << restaurants.getminheap().getheaparea()[s - 1].first.second << "\n";
					// cout << "HEAD: " << restaurants.getminheap().getheaparea()[0].first.second << "\n";
					// cout << "SWAP ID: " << restaurants.getminheap().getareas()[areas[i].first.second].heap_id << "\n";

					// locate index of swap id
					int idx = 0;
					int siz = restaurants.getminheap().getheaparea().size();
					for (int j = 0; j < siz; j++)
					{
						if (areas[i].first.second == restaurants.getminheap().getheaparea()[j].first.second)
						{
							idx = j;
							break;
						}
					}

					// cout << "---------- BEFORE SWAP 1 -----------\n";
					// for (int i = 0; i < restaurants.getminheap().getheaparea().size(); i++)
					// {
					// 	cout << restaurants.getminheap().getheaparea()[i].first.first << " " << restaurants.getminheap().getheaparea()[i].first.second << " " << restaurants.getminheap().getheaparea()[i].second << endl;
					// }
					// cout << "--------------------------------------\n";

					swap(restaurants.getminheap().getheaparea()[idx], restaurants.getminheap().getheaparea()[s - 1]);
					// swap final area with first area
					// cout << "----------- BEFORE SWAP 2 --------------\n";
					// for (int i = 0; i < restaurants.getminheap().getheaparea().size(); i++)
					// {
					// 	cout << restaurants.getminheap().getheaparea()[i].first.first << " " << restaurants.getminheap().getheaparea()[i].first.second << " " << restaurants.getminheap().getheaparea()[i].second << endl;
					// }
					// cout << "--------------------------------------\n";
					// swap(restaurants.getminheap().getheaparea()[s - 1], restaurants.getminheap().getheaparea()[0]);

					// cout << "----------- AFTER SWAP --------------\n";
					// for (int i = 0; i < restaurants.getminheap().getheaparea().size(); i++)
					// {
					// 	cout << restaurants.getminheap().getheaparea()[i].first.first << " " << restaurants.getminheap().getheaparea()[i].first.second << " " << restaurants.getminheap().getheaparea()[i].second << endl;
					// }
					// cout << "--------------------------------------\n";

					// pop the last
					restaurants.getminheap().getheaparea().pop_back();

					// restaurants.getminheap().getheaparea()[restaurants.getminheap().getareas()[0].heap_id].second = 0;
					// // remove area from heap
					// pair<pair<int, int>, int> temp = pair<pair<int, int>, int>(pair<int, int>(0, 0), areas[i].first.second);
					// cout << "need to remove: " << temp.first.first << " " << temp.first.second << " " << temp.second << endl;
					//  print heap

					// for (int i = 0; i < restaurants.getminheap().getmaxsize(); i++)
					// {
					//
					// 	cout << restaurants.getminheap().getareas()[i].heap_id << " " << restaurants.getminheap().getareas()[i].customers.size() << endl;
					// }
					// cout << "--------------------------------------\n";

					// auto it = find(restaurants.getminheap().getheaparea().begin(), restaurants.getminheap().getheaparea().end(), temp);
					// if (it != restaurants.getminheap().getheaparea().end())
					// {
					// 	// cout << "found at index " << it->first.second << endl;
					// 	restaurants.getminheap().getheaparea().erase(it);
					// }

					// for (int j = 0; i < restaurants.getminheap().getheaparea().size(); j++)
					// {
					// 	// cout
					// 	// cout << restaurants.getminheap().getheaparea()[i].first.first << " " << restaurants.getminheap().getheaparea()[i].first.second << " " << restaurants.getminheap().getheaparea()[i].second << endl;
					// 	if (restaurants.getminheap().getheaparea()[j].first.first == 0 && restaurants.getminheap().getheaparea()[j].first.second == areas[i].first)
					// 	{
					// 		restaurants.getminheap().getheaparea().erase(restaurants.getminheap().getheaparea().begin() + j);
					// 		break;
					// 	}
					// }

					// restaurants.getminheap().printheap();
				}

				// remove the customer from the queue
				// print popcustomer
				// cout << popcustomer->name << "-" << popcustomer->resultdecimal << endl;
				// restaurants.printcustomers();
				// int s = restaurants.getcustomers().size();
				// for (int z = 0; z < restaurants.getcustomers().size(); z++)
				// 	if (restaurants.getcustomers().at(z).name == popcustomer->name && restaurants.getcustomers().at(z).resultdecimal == popcustomer->resultdecimal)
				// 	{
				// 		restaurants.removecustomer(restaurants.getcustomers().at(z));
				// 		break;
				// 	}

				// find popcustomer in restaurant
				// restaurant list print
				// cout << popcustomer->name << "-" << popcustomer->resultdecimal << endl;
				// for (int j = 0; j < restaurants.getcustomers().size(); j++)
				// {
				// 	cout << restaurants.getcustomers()[j].name << "-" << restaurants.getcustomers()[j].resultdecimal << " " << (restaurants.getcustomers()[j].left == nullptr) << " " << (restaurants.getcustomers()[j].right == nullptr) << endl;
				// }

				// // find name in restaurant
				// restaurants.removecustomer(*popcustomer);

				// // cout << popcustomer->name << "-" << popcustomer->resultdecimal << endl;
				// for (int j = 0; j < restaurants.getcustomers().size(); j++)
				// {
				// 	cout << restaurants.getcustomers()[j].name << "-" << restaurants.getcustomers()[j].resultdecimal << " " << (restaurants.getcustomers()[j].left == nullptr) << " " << (restaurants.getcustomers()[j].right == nullptr) << endl;
				// }

				// restaurants.printcustomers();

				// if(areas[i].first.first == 0){
				// 	areas.erase(areas.begin() + i);
				// 	i--;
				// }

				count++;

				// reheap after each time kick
				int leaf_idx = restaurants.getminheap().getheaparea().size() / 2 - 1;
				for (int j = leaf_idx; j >= 0; j--)
					restaurants.getminheap().heapify(j);

				// cout << "-------------- HEAPIFY ----------------\n";
				// for (int j = 0; j < restaurants.getminheap().getheaparea().size(); j++)
				// {
				// 	cout << "SIZE: " << restaurants.getminheap().getheaparea()[j].first.first << " INDEX " << restaurants.getminheap().getheaparea()[j].first.second << " PRIORITY " << restaurants.getminheap().getheaparea()[j].second << endl;
				// }
				// cout << "--------------------------------------\n";
			}
			if (areas[i].first.first == 0)
			{
				areas.erase(areas.begin() + i);
				i--;
			}
			s = areas.size();
			deltime++;
		}

		// // reheap
		// int leaf_idx = restaurants.getminheap().getheaparea().size() / 2 - 1;
		// for (int i = leaf_idx; i >= 0; i--)
		// 	restaurants.getminheap().heapify(i);
	}

	void hand()
	{
		// get latest customer in customers
		if (last_customer == "")
			return;

		map<char, int> freq;
		for (char c : last_customer)
			freq[c]++;
		vector<pair<pair<char, char>, int>> vec;

		for (auto &entry : freq)
		{
			vec.push_back({{entry.first, entry.first}, entry.second});
		}

		// 3. huffman
		for (auto &entry : vec)
		{
			int shift = entry.second;
			int multiplier = static_cast<int>(entry.first.first) / 32;
			int remainder = static_cast<int>(entry.first.first) % 32;

			// entry.first.second = 'a' + (entry.first.first - 'a' + shift) % 26;
			entry.first.second = static_cast<char>(multiplier * 32 + 1 + (remainder - 1 + shift) % 26);
		}

		sort(vec.begin(), vec.end(),
			 [](const auto &lhs, const auto &rhs)
			 {
				 return lhs.first.second < rhs.first.second;
			 });

		// combine same caesar char
		int comSize = vec.size();
		for (int i = 0; i < comSize - 1; i++)
		{
			if (vec[i].first.second == vec[i + 1].first.second)
			{
				vec[i].second += vec[i + 1].second;
				vec.erase(vec.begin() + i + 1);
				i--;
			}
			comSize = vec.size();
		}

		sort(vec.begin(), vec.end(),
			 [](const auto &lhs, const auto &rhs)
			 {
				 if (lhs.second != rhs.second)
				 {
					 return lhs.second < rhs.second;
				 }
				 // in the group of same frequencys --> lower case to upper case
				 if (islower(lhs.first.second) && isupper(rhs.first.second))
					 return true;
				 else if (isupper(lhs.first.second) && islower(rhs.first.second))
					 return false;

				 // in the group of all lower case and upper case --> alphabetically
				 return lhs.first.second < rhs.first.second;
			 });

		// print map
		// cout << "------------" << endl;
		// for (auto &entry : vec)
		// {
		// 	cout << entry.first.second << ": " << entry.second << endl;
		// }

		// 3. huffman
		huffman myhuffman;
		myhuffman.buildtree(vec);
		myhuffman.printhuffman();

		myhuffman.removetree();
	}

	void limitless(int num)
	{
		if (num > restaurantg.getmaxsize() || num <= 0)
			return;
		restaurantg.getareas()[num - 1].inorder();
	}

	void cleave_helper(minheap_s min_heap, int index, int lim)
	{

		// cout << "size: " << min_heap.getheaparea().size() << endl;
		//  base case: if the index is out of bounds, return
		int s = min_heap.getheaparea().size();
		if (index >= s)
		{
			return;
		}

		// assuming the heap is stored in an array, and areas are associated with each element
		// // lifo for customers in the area
		stack<pair<int, int>> stack;
		int si = min_heap.getareas()[min_heap.getheaparea()[index].first.second].customers.size();
		for (int i = 0; i < si; i++)
		{
			stack.push(pair<int, int>(min_heap.getareas()[min_heap.getheaparea()[index].first.second].customers[i].resultdecimal, min_heap.getareas()[min_heap.getheaparea()[index].first.second].customers[i].area_id));
		}
		// stack<customer *> stack;
		// for (int i = 0; i < areas[index].customers.size(); i++)
		// {
		// 	stack.push(areas[index].customers[i]);
		// }

		// cout<<id-result
		int count = 0;
		while (!stack.empty() && count < lim)
		{
			pair<int, int> customer = stack.top();
			stack.pop();
			cout << customer.second + 1 << "-" << customer.first << endl;
			count++;
		}

		// traverse the left subtree
		// copy
		// cout << "size: " << min_heap.getheaparea().size() << endl;
		cleave_helper(min_heap, 2 * index + 1, lim);

		// cout << "size: " << min_heap.getheaparea().size() << endl;
		cleave_helper(min_heap, 2 * index + 2, lim);
	}

	void cleave(int num)
	{
		// traverse nlr min_heap
		// copy
		minheap_s min_heap = restaurants.getminheap();
		cleave_helper(min_heap, 0, num);
	}
};

void simulate(string filename)
{
	// read the file
	command mycommand;
	ifstream file(filename);
	string line;
	while (!file.eof())
	{
		getline(file, line);
		stringstream ss(line);
		string command;
		ss >> command;
		if (command == "MAXSIZE")
		{
			int size;
			ss >> size;
			mycommand.setmaxsize(size);
		}
		else if (command == "LAPSE")
		{
			string name;
			ss >> name;
			if (name.length() < 3)
				continue;
			mycommand.lapse(name);
		}
		else if (command == "HAND")
		{
			mycommand.hand();
			// handle hand
		}
		else if (command == "LIMITLESS")
		{
			int num;
			ss >> num;
			mycommand.limitless(num);
			// handle limitless
		}
		else if (command == "KEITEIKEN")
		{
			int num;
			ss >> num;
			mycommand.keiteiken(num);
			// handle keiteiken
		}
		else if (command == "KOKUSEN")
		{
			mycommand.kokusen();
			// handle kokusen
		}
		else if (command == "CLEAVE")
		{
			// handle cleave
			int num;
			ss >> num;
			mycommand.cleave(num);
		}
	}

	// free memory
	// restaurant g
	for (int i = 0; i < mycommand.getrestaurantg().getmaxsize(); i++)
	{
		mycommand.getrestaurantg().getareas()[i].removebst();
	}

	// delete areas
	mycommand.getrestaurantg().killArea();
	mycommand.getrestaurants().getminheap().killArea();

	// restaurant s
	// int ms = mycommand.getrestaurants().getminheap().getmaxsize();
	// for (int i = 0; i < ms; i++)
	// {
	// 	// delete customer
	// 	int noCus = mycommand.getrestaurants().getminheap().getareas()[i].customers.size();
	// 	for (int j = 0; j < noCus; j++)
	// 	{
	// 		// delete mycommand.getrestaurants().getminheap().getareas()[i].customers[j];
	// 		// mycommand.getrestaurants().getminheap().getareas()[i].customers[j] = nullptr;
	// 	}
	// }
}