import java.util.Scanner;
public class HW2_3 {
	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);
		int row =0, col=0;
		System.out.print("Number of rows and colums ");
		row=sc.nextInt(); col=sc.nextInt(); 
		
		int[][] ent=new int[row+1][col+1];
		
		System.out.println("Input the entries");
		for(int i=0;i<row;i++) {
			for(int j=0;j<col;j++) {
				ent[i][j]=sc.nextInt();
			}
		}
		
		for(int i=0;i<row;i++) {
			for(int j=0;j<col;j++) {
				ent[i][col]=ent[i][col]+ent[i][j];
			}
		}
		for(int i=0;i<col;i++) {
			for(int j=0;j<row;j++) {
				ent[row][i]=ent[row][i]+ent[j][i];
			}
		}
		
		for(int i=0;i<row+1;i++) {
			for(int j=0;j<col+1;j++) {
				if(j==col&&i==row)
					System.out.println("");
				else
					System.out.printf("%4d", ent[i][j]);
			}
			System.out.println("");
		}
	}
}
