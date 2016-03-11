
/**
 * Implementation of a Red-Black tree.
 * Supported operations: 
 * <li>insert(node x)
 * <li>delete(node x)
 * @author Youri
 *
 * @param <K>
 */
public class RBTree<K> {
	int key;
	K value;
	RBTree<K> left;
	RBTree<K> right;
	RBTree<K> parent;
	int size;
	int rank;
	boolean color;
	
	public RBTree() {
		
	}
	
	public RBTree(int k, K v, RBTree<K> l, RBTree<K> r, RBTree<K> p, int s, int rk, boolean c) {
		key = k;
		value = v;
		left = l;
		right = r;
		parent = p;
		size = s;
		rank = rk;
		color = c;
	}
	
		public static RBTree makeRBTree(int[] A) {
		return makeRBTree(A, 0, A.length-1);
	}
	
	public static RBTree makeRBTree(int[] A, int start, int end) {
		RBTree root = new RBTree();
		int mid = start+(end-start)/2;
		
		if (end <= start) return null;
		else {
			root.key = A[mid];
			root.left = makeRBTree(A, start, mid);
			root.right = makeRBTree(A, mid+1, end);
			
			if ((root.left != null && root.left.color) || (root.right != null && root.right.color)) {
				root.color = false;
			}
			else root.color = true;
			
			if (root.left == null || root.right == null) root.rank = 1;
			else if (root.left.color) root.rank = root.left.rank;
			else if (root.right.color) root.rank = root.right.rank;
			else root.rank = root.left.rank + 1;
		}
		
		return root;
	}
	
	public void insert(RBTree<K> x) {
		if (x.key < this.key && this.left == null) {
			this.left = x;
			x.parent = this;
			x.color = true;
			x.rank = 1;
			fix(x);
		}
		else if (x.key > this.key && this.right == null) {
			this.right = x;
			x.parent = this;
			x.color = true;
			x.rank = 1;
			fix(x);
		}
		else if (x.key < this.key) {
			this.left.insert(x);
		}
		else if (x.key > this.key) {
			this.right.insert(x);
		}
		else {
			/* Need to throw Exception in this case (x.key = this.key) */
		}
	}
	
	public void delete(RBTree<K> x) {
		
	}
	
	public static void fix(RBTree x) {
		RBTree uncle;
		
		if (x.parent == null || x.parent.parent == null) return;
		//Note that a RB tree is supposed to have a black root.
		
		uncle = x.parent.parent.otherChild(x.parent);
		
		if (!uncle.color) {
			rotate(x, uncle);
			return;
		}
		else {
			x.parent.color = false;
			uncle.color = false;
			uncle.parent.color = true;
			uncle.parent.rank = uncle.rank + 1;
			fix(uncle.parent);
		}
		
	}
	
	public static void rotate(RBTree x, RBTree u) {
		x.parent.parent = u.parent.parent;
		
		u.parent.parent = x.parent;
		
		
	}
	
	private RBTree<K> otherChild(RBTree<K> x) {
		if (this.left == x) return this.right;
		else return this.left;
	}
	
}
