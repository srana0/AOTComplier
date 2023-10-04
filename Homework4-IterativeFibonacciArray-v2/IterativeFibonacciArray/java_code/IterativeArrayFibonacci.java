public class IterativeArrayFibonacci{
    public static void theThing(int d1, int d2, int d3, int d4,int value,int[] a){
        int one = 1;
        int two = 2;
        a[0] = 0;
        a[one] = one;
        for (int i = two ;  i-value - one < 0; i++){
            a[i] = a[i-one] + a[i-two];
        }
    }
    public static void clearArray(int d1, int d2, int d3, int d4,int[] a, int size){
        for (int i = 0 ;  i-size  <= 0; i++){
            a[i]=0;
            System.out.println("i is "+i);
        }
    }
    public static void main (String[] args){
        int padding1; int padding2;int padding3;int padding4;
        System.out.println("Hello");
        int one = 1;
        int oft = 66 ;
        int pft = oft * 125;
        int[] array = new int[46];
        for (int i = 0 ; i - pft <= 0 ; i++){
            clearArray(0,0,0,0,array,45);
            for (int j = 0 ; j - 45 < 0; j++){
                theThing(0,0,0,0,j,array);
            }

        }
        
        for (int i = 0; i -46 < 0; i++){
            System.out.println(array[i]);
        }
    }
}