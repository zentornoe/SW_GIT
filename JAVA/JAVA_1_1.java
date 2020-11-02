package ex;
import java.util.Scanner;
public class HW14 {

	public static void main(String[] args) {
		Scanner input = new Scanner(System.in);
		System.out.print("Enter a natural number. : ");
		int x=input.nextInt();
		
		if(x>0) {
			for(int i=0;i<x;i++) {
				for(int j=1;j<x-i;j++) {
					System.out.print(" ");
				}
				int y=(i*2+1)/9, z=(2*i+1)%9; 
				if(z==0&&y>0) {
					for(int k=0;k<y;k++) {
						System.out.printf("987654321");
					}
				}
				else if(z!=0&&y==0) {
					for(int l=z;l>0;l--) {
						System.out.print(l);
					}
				}
				else {
					for(int p=z;p>0;p--) {
						System.out.print(p);
					}
					for(int o=0;o<y;o++) {
						System.out.printf("987654321");
					}
				}
				System.out.println("");
			}		
		}
	}
}
