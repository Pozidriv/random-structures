/** KD Tree implementation.
* Author: Youri Tamitegama
*
* On average:
* Insert in log n
* Build in n log n
* NNS in log n
*
* Remove not finished.
* findMin not finished
*/
#pragma once
#include <cstddef>
#include <cmath>

using namespace std;

double L2_dist_squared(vector<double> x, vector<double> y, int dim) {
	//if (x.size() != y.size()) return -1;
	int i = 0;
	double sum = 0;

	vector<double>::iterator it_x;
	vector<double>::iterator it_y;
	for (it_x = x.begin(), it_y = y.begin();
		it_x < x.end(), it_y < y.end(), i < dim;
		it_x++, it_y++, i++) {
		sum += (*it_x - *it_y) * (*it_x - *it_y);
	}
	return sum;
}

double L2_dist(vector<double> x, vector<double> y, int dim) {
	return sqrt(L2_dist_squared(x, y, dim));
}

template<class R> class KDTree;

template<class R>
struct nodeDist {
	KDTree<R>* tree;
	double dist;
};
template<class R>
nodeDist<R>* get_min(vector<nodeDist<R>>* list);
template<class R>
nodeDist<R>* get_max(vector<nodeDist<R>>* list, int num);
bool are_equal(vector<double> a, vector<double> b, int k);
double randomDouble(void);


template <class R>
class KDTree{

public:
	vector<double> coordinates;
	R value;
	int dimensions;

	KDTree();
	KDTree(vector<double> coordinates, R value, int dim);
	KDTree(vector<vector<double>> points, vector<R> values, int dim);
	KDTree(KDTree* left, KDTree* right, vector<double> coordinates, R value, int dim);

	KDTree<R>* insert(vector<double> coordinates, R element, int cd);
	KDTree<R>* remove(vector<double> loc, int cd);
	KDTree<R>* findMin(int axis, int cd);
	vector<nodeDist<R>> nearestNeighbors(KDTree<R> &searchNode, int num);
	KDTree<R>* minimum(KDTree<R> &tree1, KDTree<R> &tree2, int axis);
   bool isEqual(KDTree<R>& a);

	void undensify(KDTree<R> &root, int k, double density, double remove_prob, int cd);

	pair<vector<vector<double> >, vector<R>> to_vector();
	int DFS_with_prints(int count);
	int DFS_with_prints(ofstream* out, int count);
	void print_coord();
	void print_coord(ofstream* out);
	~KDTree();


private:
	KDTree* parent;
	KDTree* left;
	KDTree* right;


	void add_if_better(KDTree<R> &searchNode, vector<nodeDist<R>>* ref, int num, double distance);
	pair<vector<nodeDist<R>>, KDTree<R>*> nearestNeighbors(KDTree<R> &searchNode, int num, vector<nodeDist<R>> refNodes, int cd);
};

template <class R>
KDTree<R>::KDTree() {

}

template <class R>
KDTree<R>::KDTree(vector<vector<double>> points, vector<R> values, int dim) {
	cout << "Building a new KDTree of size " << points.size() << endl;
	this->coordinates = vector<double>();
	coordinates.swap(points.back());
	points.pop_back();
	this->value = values.back();
	values.pop_back();

	this->left = nullptr;
	this->right = nullptr;
	this->dimensions = dim;
	cout << "Root is ready" << endl;

	while (!points.empty()) {
		//cout << "Current, L: " << left << "; R: " << right << endl;
		//cout << "Inserting node" << endl;

		KDTree<R>* test = this->insert(points.back(), values.back(), 0);
		points.pop_back();
		values.pop_back();

      cout << ".";
		//cout << "Node inserted: ";
		//test->print_coord(cout, dimensions);
	}
   cout << endl;
}

template <class R>
KDTree<R>::KDTree(vector<double> coordinates, R value, int dim) {

	this->left = nullptr;
	this->right = nullptr;
	this->coordinates = vector<double>();
	this->coordinates.swap(coordinates);
	this->value = value;
	this->dimensions = dim;
}

template <class R>
KDTree<R>::KDTree(KDTree<R>* left, KDTree<R>* right, vector<double> coordinates, R value, int dim) {
	this->left = left;
	this->right = right;
	this->coordinates = coordinates;
	this->value = value;
	this->dimensions = dim;

}

// Insert in log(n)
template <class R>
KDTree<R>* KDTree<R>::insert(vector<double> loc, R element, int cd) {
	if (are_equal(loc, coordinates, dimensions)) {
		cout << "Duplicate" << endl;
		return this;
	}
	if (coordinates[cd] > loc[cd]) {
		if (left == nullptr) {
			left = new KDTree<R>(loc, element, dimensions);
			left->parent = this;
			return left;
		}
		return left->insert(loc, element, (cd + 1) % dimensions);
	}
	if (right == nullptr) {
		right = new KDTree<R>(loc, element, dimensions);
		right->parent = this;
		return right;
	}
	return right->insert(loc, element, (cd + 1) % dimensions);
}

template <class R>
KDTree<R>* KDTree<R>::findMin(int axis, int cd) {
	if (axis == cd % dimensions) {
		if (left == nullptr) {
			return this;
		}
		else {
			return left->findMin(axis, cd + 1);
		}
	}
	if (right != nullptr) {
		return this->minimum(*left->findMin(axis, cd + 1), *right->findMin(axis, cd + 1), axis);
	}
	else {
		return this->minimum(*left->findMin(axis, cd + 1), *this, axis);
	}
}

template <class R>
KDTree<R>* KDTree<R>::minimum(KDTree<R> &tree1, KDTree<R> &tree2, int axis) {
	if (tree1.coordinates[axis] < tree2.coordinates[axis] && tree1.coordinates[axis] < coordinates[axis])
		return &tree1;
	if (tree2.coordinates[axis] < tree1.coordinates[axis] && tree2.coordinates[axis] < coordinates[axis])
		return &tree2;
	if (coordinates[axis] < tree2.coordinates[axis] && coordinates[axis] < tree1.coordinates[axis])
		return this;
	return this;
}

// TO TEST
template <class R>
KDTree<R>* KDTree<R>::remove(vector<double> loc, int cd) {
	// If is point
	if (are_equal(loc, coordinates, dimensions)) {
		KDTree<R>* min_axis = nullptr;
		if (this->left == nullptr && this->right == nullptr) {	// Leaf case
			if (parent->left == this) parent->left = nullptr;
			if (parent->right == this) parent->right = nullptr;
		}
		else if (right != nullptr) {							// If right not null
			min_axis = right->findMin(cd % dimensions, cd);
		}
		else if (left != nullptr) {							// Otherwise
			min_axis = left->findMin(cd % dimensions, cd);
		}
		remove(min_axis->coordinates, cd);

		// Replace this with node found.
		if (parent->left == this) parent->left = min_axis;
		if (parent->right == this) parent->right = min_axis;
		min_axis->left = left;
		min_axis->right = right;
		min_axis->parent = parent;
		left = nullptr;
		right = nullptr;
		parent = nullptr;
	}

	// Point not found
	if (this->left == nullptr && this->right == nullptr) {
		return nullptr;
	}

	// Recursion
	if (loc[cd % dimensions] < coordinates[cd % dimensions]) {
		return left->remove(loc, cd + 1);
	}
	else {
		return right->remove(loc, cd + 1);
	}
}

// Compares k first elements of a and b
bool are_equal(vector<double> a, vector<double> b, int k) {
	if (a.size() < k || b.size() < k) return false;
	for (int i = 0; i < k; i++) {
		if (a[i] != b[i]) return false;
	}
	return true;
}

template <class R>
vector<nodeDist<R>> KDTree<R>::nearestNeighbors(KDTree<R> &searchNode, int num) {
	vector<KDTree<R>*> nn;
	vector<nodeDist<R>> temp;

	temp = this->nearestNeighbors(searchNode, num, temp, 0).first;

	typename vector<nodeDist<R>>::iterator it;
	for (it = temp.begin(); it < temp.end(); it++) {
		nn.push_back((*it).tree);
	}

	return temp;
}


template <class R>
pair<vector<nodeDist<R>>, KDTree<R>*> KDTree<R>::nearestNeighbors(KDTree<R> &searchNode, int num, vector<nodeDist<R>> refNodes, int cd) {
	if (num == 0) return make_pair(refNodes, this);

	double distance = L2_dist(coordinates, searchNode.coordinates, dimensions);

	// Find "closest" leaf node
	pair<vector<nodeDist<R>>, KDTree<R>*> child_res;
	if (coordinates[cd % dimensions] > searchNode.coordinates[cd % dimensions]) {
		if (left == nullptr) {
			this->add_if_better(searchNode, &refNodes, num, distance);
		}
		else {
			child_res = left->nearestNeighbors(searchNode, num, refNodes, cd + 1);
			refNodes.swap(child_res.first);
			this->add_if_better(searchNode, &refNodes, num, distance);
		}
	}
	else if (right == nullptr) {
		this->add_if_better(searchNode, &refNodes, num, distance);
	}
	else {
		child_res = right->nearestNeighbors(searchNode, num, refNodes, cd + 1);
		refNodes.swap(child_res.first);
		this->add_if_better(searchNode, &refNodes, num, distance);
	}

	typename vector<nodeDist<R>>::iterator node_it;


	// Leaf case
	if (left == nullptr && right == nullptr) {
		return make_pair(refNodes, this);
	}

	nodeDist<R>* max = get_max(&refNodes, num);
	double max_dist;
	if (max != nullptr) {
		max_dist = max->dist;
	}
	else {
		max_dist = 1000000000000000;
	}

	// Recursion
	// Explore right first
	if (coordinates[cd % dimensions] < searchNode.coordinates[cd % dimensions]) {
		if (coordinates[cd % dimensions] + max_dist > searchNode.coordinates[cd % dimensions]
			&& right != nullptr && child_res.second != right) {
			vector<nodeDist<R>> tmp = right->nearestNeighbors(searchNode, num, refNodes, cd + 1).first;
			refNodes.swap(tmp);
		}
		if (coordinates[cd % dimensions] - max_dist < searchNode.coordinates[cd % dimensions]
			&& left != nullptr && child_res.second != left) {
			vector<nodeDist<R>> tmp = left->nearestNeighbors(searchNode, num, refNodes, cd + 1).first;
			refNodes.swap(tmp);
		}

		// Explore left first
	}
	else {
		if (coordinates[cd % dimensions] - max_dist < searchNode.coordinates[cd % dimensions]
			&& left != nullptr && child_res.second != left) {
			vector<nodeDist<R>> tmp = left->nearestNeighbors(searchNode, num, refNodes, cd + 1).first;
			refNodes.swap(tmp);
		}
		if (coordinates[cd % dimensions] + max_dist > searchNode.coordinates[cd % dimensions]
			&& right != nullptr && child_res.second != right) {
			vector<nodeDist<R>> tmp = right->nearestNeighbors(searchNode, num, refNodes, cd + 1).first;
			refNodes.swap(tmp);
		}
	}
	return make_pair(refNodes, this);
}

template <class R>
void KDTree<R>::add_if_better(KDTree<R> &searchNode, vector<nodeDist<R>>* refNodes, int num, double distance) {
	if (this != &searchNode) {
		// Update closest points if necessary
		if (refNodes->size() < num) {
			refNodes->push_back({ this, distance });
		}
		else {
			nodeDist<R>* max_node = get_max(refNodes, num);
			if (max_node == nullptr) {
				//				cout << "wut ";
			}
			else if (max_node->dist > distance) {
				*max_node = { this, distance };
			}
		}
	}

}

// Removing points that are too densly distributed with a certain probability
template <class R>
void KDTree<R>::undensify(KDTree<R> &root, int k, double density, double remove_prob, int cd) {
	vector<nodeDist<R>> tmp = root.nearestNeighbors(*this, k);
	double distance = tmp[0].dist;
	if (distance < density) {
		if (randomDouble() < remove_prob) {
			this->remove(this->coordinates, cd);
		}
	}

	if (left != nullptr) left->undensify(root, k, density, remove_prob, cd + 1);
	if (right != nullptr) right->undensify(root, k, density, remove_prob, cd + 1);
}

template <class R>
void KDTree<R>::print_coord() {
	cout << "(";
	vector<double>::iterator it = coordinates.begin();

	for (; it < coordinates.end() - 1; it++) {
		cout << " " << *it << ",";
	}
	cout << " " << *it << " ";
	cout << ")" << endl;
}

template <class R>
void KDTree<R>::print_coord(ofstream* out) {
	//	*out << "(";
	vector<double>::iterator it = coordinates.begin();

	for (; it < coordinates.end() - 1; it++) {
		*out << *it << ",";
	}
	*out << " " << *it;
	*out << endl;
}

template <class R>
pair<vector<vector<double>>, vector<R>> KDTree<R>::to_vector() {
	vector<vector<double>> coord;
	vector<R> values;

	coord.push_back(coordinates);
	values.push_back(value);

	pair<vector<vector<double>>, vector<R>> leftpair;
	pair<vector<vector<double>>, vector<R>> rightpair;
	if (left != nullptr) leftpair = left->to_vector();
	if (right != nullptr) rightpair = right->to_vector();

	coord.insert(coord.begin(), leftpair.first.begin(), leftpair.first.end());
	coord.insert(coord.begin(), rightpair.first.begin(), rightpair.first.end());
	values.insert(values.begin(), leftpair.second.begin(), leftpair.second.end());
	values.insert(values.begin(), rightpair.second.begin(), rightpair.second.end());

	return make_pair(coord, values);
}

template <class R>
int KDTree<R>::DFS_with_prints(int count) {
	cout << count << ": ";
	print_coord();

	int new_count = 0;
	if (left != nullptr) {
		new_count = left->DFS_with_prints(count + 1);
	}
	else {
		new_count = count + 1;
	}


	int final_count = 0;
	if (right != nullptr) {
		final_count = right->DFS_with_prints(new_count);
	}
	else {
		final_count = new_count;
	}

	return final_count;
}

template <class R>
int KDTree<R>::DFS_with_prints(ofstream* out, int count) {
	*out << count << " ";
	print_coord(out);

	int new_count = 0;
	if (left != nullptr) {
		new_count = left->DFS_with_prints(out, count + 1);
	}
	else {
		new_count = count + 1;
	}

	int final_count = 0;
	if (right != nullptr) {
		final_count = right->DFS_with_prints(out, new_count);
	}
	else {
		final_count = new_count;
	}

	return final_count;
}

template <class R>
KDTree<R>::~KDTree() {
	delete left;
	delete right;
}

template<class R>
struct nodeIter {
	KDTree<R>* tree;
	int count;
};

template<class R>
nodeDist<R>* get_max(vector<nodeDist<R>>* list, int num) {
	double best = -1;
	nodeDist<R>* ptr = nullptr;

	typename vector<nodeDist<R>>::iterator it;
	for (it = list->begin(); it < list->end(); it++) {
		if (best < (*it).dist) {
			best = (*it).dist;
			ptr = &(*it);
		}
	}
	if (list->size() < num) return nullptr;
	return ptr;
}

template<class R>
nodeDist<R>* get_min(vector<nodeDist<R>>* list) {
	double best = 10000000000000000;
	nodeDist<R>* ptr = nullptr;

	typename vector<nodeDist<R>>::iterator it;
	for (it = list->begin(); it < list->end(); it++) {
		if (best >(*it).dist) {
			best = (*it).dist;
			ptr = &(*it);
		}
	}
	if (list->size() == 0) return nullptr;
	return ptr;
}

template<class R>
KDTree<R>* lazy_remove(vector<double> loc, int cd, bool flag) {
   if (!flag) { // Recursively find

   } else { // delete and reform tree

   }

}

template <class R>
bool KDTree<R>::isEqual(KDTree<R>& a) {
   if (!are_equal(a.coordinates, coordinates, dimensions)) return false;
   if (left == nullptr) {
      if (a.left == nullptr) return true;
      else return false;
   }
   if (a.left == nullptr) {
      if (left == nullptr) return true;
      else return false;
   }
   if (right == nullptr) {
      if (a.right == nullptr) return true;
      else return false;
   }
   if (a.right == nullptr) {
      if (right == nullptr) return true;
      else return false;
   }

   return (left->isEqual(*a.left) && right->isEqual(*a.right));
}

