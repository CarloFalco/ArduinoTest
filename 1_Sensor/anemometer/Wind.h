
class Anemometer {
  public:
    Anemometer(int pin, float diameter);
    void begin();
    void update();
    String getDirection(float windSpeed_kph);
    
  private:
    int _pin;
    float _diameter;
    volatile int _rotationCount;
    int _rotationCounts[numElements];
    int _arrayIndex;
    TaskHandle_t _taskHandle;

    static void countRevolutions();
    static void anemometerTask(void* pvParameters);
};
