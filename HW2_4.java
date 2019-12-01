
public class HW2_4 {
	public static void main(String[] args) {
		int i;
		int[] ent= new int[10];
		
		
		for(i=0;i<50;i++) {
			int num = (int)(Math.random()*101);
			
			switch(num/10) {
			case 10 :
			case 9 : ent[9]++; break;
			case 8 : ent[8]++; break;
			case 7 : ent[7]++; break;
			case 6 : ent[6]++; break;
			case 5 : ent[5]++; break;
			case 4 : ent[4]++; break;
			case 3 : ent[3]++; break;
			case 2 : ent[2]++; break;
			case 1 : ent[1]++; break;
			case 0 : ent[0]++; break;
			}
		}
		
		int max=0;
		for(int j=0;j<10;j++) {
			if(max<=ent[j])
				max = ent[j];
		}
		
		for(int j=max;j>0;j--) {
			for(int k=0;k<10;k++) {
				if(ent[k]==j) {
					System.out.print(" * "); ent[k]--;
				}
				else if(ent[k]!=j)
					System.out.print("   ");
			}
			System.out.println("");
		}
		for(int j=0;j<10;j++) {
			System.out.printf("%3d", j*10+5);
		}
	}
}
