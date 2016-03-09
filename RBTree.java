
/**
 * Implementation of a Red-Black tree.
 * Supported operations: 
 * <li>Insert(node x)
 * <li>Delete(node x)
 * @author Youri
 *
 * @param <K>
 */
public class RBTree<K> {
	int key;
	K value;
	BinaryTree<K> left;
	BinaryTree<K> right;
	int size;
	int rank;
	boolean color;
	
	public RBTree() {
		
	}
	
	public RBTree(int k, K v,	BinaryTree<K> l, BinaryTree<K> r, int s, int rk, boolean c) {
		key = k;
		value = v;
		left = l;
		right = r;
		size = s;
		rank = rk;
		color = c;
	}
	
	public void insert(BinaryTree<K> x) {
		
	}
	
	public void delete(BinaryTree<K> x) {
		
	}
}
