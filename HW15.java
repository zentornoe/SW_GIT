package ex;
import java.util.Scanner;
public class HW15 {
	public static void main(String args[]) {
		Scanner input = new Scanner(System.in);
		System.out.print("Enter a natural number. ");
		int x = input.nextInt();
		String a="123456789", b="987654321", c="87654321";
		
		if(x>0) {
			for(int i=0;i<x;i++) {
				for(int j=1;j<x-i;j++) {
					System.out.print(" ");
				}
				int y=(i+1)/9, z=(i+1)%9;
				if(z==0&&y>0) {
					for(int k=0;k<y;k++) {
						System.out.print(a);
					}
					System.out.print(c);
					for(int l=1;l<y;l++) {
						System.out.print(b);
					}
				}
				else if(z!=0&&y==0) {
					for(int m=1;m<z;m++) {
						System.out.print(m);
					}
					for(int n=z;n>0;n--) {
						System.out.print(n);
					}
				}
				else {
					for(int o=0;o<y;o++) {
						System.out.print(a);
					}
					for(int p=1;p<z;p++) {
						System.out.print(p);
					}
					for(int q=z;q>0;q--) {
						System.out.print(q);
					}
					for(int r=0;r<y;r++) {
						System.out.print(b);
					}
				}
				System.out.println("");
			}
		}
	}
}
