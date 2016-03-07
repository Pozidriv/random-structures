
/**
 * Slightly augmented implementation of a binary tree.
 * Each element in the tree has the following fields:
 * <li><code>key</code>, a non-zero integer unique to this particular node (mainly used similarly to a pointer)
 * <li><code>value</code>, an Object representing the value of that node
 * <li><code>left</code>, the root of the left subtree of this node
 * <li><code>right</code>, the root of the right subtree of this node
 * <li><code>size</code>, the number of elements of the tree rooted at this node
 * </li>
 * <p><b>!!!WARNING!!!</b> Do NOT create loops within the tree! 
 * It will no longer be a proper tree and thus most methods will cause problems.
 * @author Pozidriv
 *
 * @param <K> type of the field <code>value</code>
 */
public class BinaryTree<K> {
	int key;
	K value;
	BinaryTree<K> left;
	BinaryTree<K> right;
	int size;
	
	/**
	 * Default constructor.
	 */
	public BinaryTree() {
		
	}
	
	/**
	 * Basic constructor.
	 * @param k key
	 * @param v value
	 * @param l left
	 * @param r right
	 */
	public BinaryTree(int k, K v, BinaryTree<K> l, BinaryTree<K> r, int s) {
		key = k;
		value = v;
		left = l;
		right = r;
		size = s;
	}
	
	
	/************************* Basic operations *****************************/
	
	public boolean isEmpty() {
		return (this == null || this.size == 0);
	}
	
	/**
	 * DFS to find a node of key <b>k</b>.
	 * @param k the key of the node to find in the tree
	 * @return the BinaryTree with key k.
	 */
	public BinaryTree<K> find(int k) {
		BinaryTree<K> l,r;
		
		if (this.key == k) return this;
		else {
			l = this.left.find(k);
			r = this.right.find(k);
			
			if (l == null) return r;
			else return right;
		}
	}
	
	/**************************** Traversals ********************************/
	
	/**
	 * All traversals take in input a binary tree node: the root of the tree to be traversed.
	 * They return an array of integers representing the keys of each node of the tree.
	 * The order of the array thus depends on the type of traversal.
	 */
	
	/**
	 * Pre-order traversal.
	 * @return
	 */
	public int[] PreTraversal() {
		int[] output;
		output = new int[this.size];
		
		visit(output, this.key);
		if (!this.left.isEmpty()) left.PreTraversal();
		if (!this.right.isEmpty()) right.PreTraversal();
		
		return output;
	}
	
	/**
	 * In-order traversal.
	 * @return
	 */
	public int[] InTraversal() {
		int[] output;
		output = new int[this.size];
		
		if (!this.left.isEmpty()) left.InTraversal();
		visit(output, this.key);
		if (!this.right.isEmpty()) right.InTraversal();
		
		return output;
	}
	
	/**
	 * Pre-order traversal.
	 * @return
	 */
	public int[] PstTraversal() {
		int[] output;
		output = new int[this.size];
		
		if (!this.left.isEmpty()) left.PstTraversal();
		if (!this.right.isEmpty()) right.PstTraversal();
		visit(output, this.key);
		
		return output;
	}
	
	/************************** Helper methods *******************************/
	
	public void visit(int[] storage, int k) {
		for (int i = 0; i < 0; i++) {
			if (storage[i] == 0) {
				storage[i] = k;
				return;
			}
		}
	}
}
