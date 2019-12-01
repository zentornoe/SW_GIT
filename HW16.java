package ex;
import java.util.Scanner;
public class HW16 {

	public static void main(String[] args) {
		Scanner input = new Scanner(System.in);
		System.out.print("Enter a natural number. ");
		int x=input.nextInt();
		int i, j, k, l, m, n, o, p;
		
		for(i=0;i<x;i++) {
			int w=(i+1)%2, u=i*i%9;
			if(u==0)
				u=9;		
			int z=((2*i+1)-(9-u))/9, y=((2*i+1)-(9-u))%9; //¼±¾ð
			
			if(u==0)
				u=9;
				

			if(y<0)
				y=0;
			

			
			for(j=1;j<x-i;j++) {
				System.out.print(" ");
			}
			
			if(w==1) {
				if(i==0) {
					System.out.print("1");
				}
				else if(i==2) {
					System.out.print("56789");
				}
				else {
					for(k=u+1;k<10;k++) {
						System.out.print(k);
					}
					for(l=0;l<z;l++) {
						System.out.print("123456789");
					}
					for(m=1;m<y+1;m++) {
						System.out.print(m);
					}
				}
			}
			
			else {
				if(i==1) {
					System.out.print("432");
				}
				else if(i==3) {
					System.out.print("7654321");
				}
				else {
					for(k=y;k>0;k--) {
						System.out.print(k);
					}
					for(l=0;l<z;l++) {
						System.out.print("987654321");
					}
					if(u!=0) {
						for(m=9;m>u;m--) {
							System.out.print(m);
						}
					}
				}
			}
			System.out.println("");	
		}
	}
}