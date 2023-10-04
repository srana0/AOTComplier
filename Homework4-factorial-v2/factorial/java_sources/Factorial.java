public class Factorial{
public static int factorial(int padding1, int padding2,int padding3,int padding4, int n){
    if ( n - 2 < 0 ) return 1;
    return n*factorial(0,0,0,0,n-1);
}
public static int factorialIter(int padding1, int padding2,int padding3,int padding4, int n){
    int result = 1;
    for (int i = 2 ; i - n <= 0; i++){
        result = result * i;
    }
    return result;
}
public static void main(String[]args){
    int padding1; int padding2;int padding3;int padding4;
    int one = 1;
    int oft = 66 ;
    int pft = oft * 125;
    for (int i = 0 ; i - pft <= 0 ; i++){
        for (int j = 0 ; j - 12 < 0; j++){
            int bonus = i*12+j;
            int result = factorial(0,0,0,0,j+one);
            System.out.println(bonus);
            System.out.println(result);
        }
    }
    for (int i = 0 ; i - pft <= 0; i++){
        for (int j = 0 ; j - 12 < 0 ; j++){
            int bonus = i*12+j;
            int result = factorialIter(0,0,0,0,j+one);
            System.out.println(bonus);
            System.out.println(result);
        }
    }
}
}