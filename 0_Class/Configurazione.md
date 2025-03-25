nella cartella 3 c'e una definizione di un oggetto tutto nel file .h, nel 3bis viene fatto diviso tra .cpp e .h


private

I membri dichiarati come private sono accessibili solamente all'interno della classe stessa in cui sono definiti.
Nessuna classe derivata può accedere direttamente a questi membri; l’unico modo per manipolarli dall'esterno è tramite metodi pubblici o protetti della classe.
Questo livello di incapsulamento è utile per garantire che determinati dati non vengano accidentalmente modificati dalle sottoclassi.


protected

I membri dichiarati come protected sono accessibili sia all'interno della classe stessa sia nelle classi derivate.
Tuttavia, rimangono inaccessibili dall'esterno della gerarchia di classi (cioè, dal codice che non appartiene né alla classe base né alle classi derivate).
Questo livello di accesso è spesso utilizzato quando si desidera che le sottoclassi possano accedere e modificare determinati attributi o metodi senza renderli disponibili pubblicamente.



class Base {
private:
    int privateVar = 10;  // Accessibile solo all'interno di Base

protected:
    int protectedVar = 20; // Accessibile in Base e nelle classi derivate

public:
    int publicVar = 30;  // Accessibile ovunque
};

class Derived : public Base {
public:
    void test() {
        // privateVar = 15;  // ❌ Errore: non è accessibile
        protectedVar = 25;   // ✅ Accessibile perché è protected
        publicVar = 35;      // ✅ Accessibile perché è public
    }
};

int main() {
    Derived d;
    // d.privateVar = 40;  // ❌ Errore: non è accessibile
    // d.protectedVar = 50; // ❌ Errore: accessibile solo alle sottoclassi, non dall'esterno
    d.publicVar = 60;      // ✅ Accessibile
}
