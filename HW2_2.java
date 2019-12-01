import java.util.Scanner;

public class HW2_2 {
	
	int[][] sdk = new int[9][9]; public final int s=45; //sum(=sum from 1 to 9), map
	
	public static void main(String[] args) {
		HW2_2 SD = new HW2_2();
		Scanner sc = new Scanner(System.in);
		
		
		System.out.println("Input the map.");
		
		for(int i=0;i<9;i++) {
			for(int j=0;j<9;j++) {
				for(int k=0;k<9;k++) {
					SD.sdk[j][k]=sc.nextInt(); //Input
				}
			}	
			
			if(SD.isCol()&&SD.isRow()&&SD.isBox())
				System.out.println("VALID");
			else
				System.out.println("INVALID");
		}
		
		
		
	}
	
	public boolean isCol() {
		for(int i=0;i<9;i++) {
			int sum=0;
			for(int j=0;j<9;j++) { sum=sum+sdk[i][j]; }
			if(sum!=s)
				return false;
		}
		return true;
	}//Column
	
	
	public boolean isRow() {
		for(int i=0;i<9;i++) {
			int sum=0;
			for(int j=0;j<9;j++) sum=sum+sdk[j][i];
			if(sum!=s)
				return false;
		}
		return true;
	}//Row
	
	public boolean isBox() {
		int a=0, b=0, i, j, k;
		
		for(i=0;i<9;i++) {
			int sum=0;			
			for(j=b;j<b+3;j++)
				for(k=a;k<a+3;k++) { sum=sum+sdk[j][k]; }
								
					if(sum!=s) return false; //is not sum from 1 to 9
					
					if(a<4)
						a=a+3; //right side box
					else { b=b+3; a=0; } //lower row's box
			if(b>7) break; //last 9th box => end.
		}
		return true;		
	}	
}