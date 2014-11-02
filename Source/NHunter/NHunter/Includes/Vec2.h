#ifndef VEC2_H
#define VEC2_H


class Vec2
{
public:
        double x, y;

        Vec2() : x(0), y(0){ }
        Vec2(double a, double b) { x=a; y=b; }
        Vec2(int a, int b) { x=a; y=b; }
        ~Vec2(){ };

        Vec2& operator-();

        bool operator==(Vec2 v);
        bool operator!=(Vec2 v);

        Vec2  operator+(Vec2 v);      
        Vec2  operator-(Vec2 v);      
        Vec2& operator+=(Vec2 v);     
        Vec2& operator-=(Vec2 v);     

        double operator*(Vec2 v);        
        Vec2 operator*(double s);       
        Vec2 operator/(double s);     
        void Rotate(double radians);
		Vec2 Vec2::vRotate(double radians);

        Vec2 Normalize(){ return *this * (1/Magnitude()); }
        double Magnitude() const;			
        double Argument() const;		
        double Distance(Vec2 v) const;    
};

Vec2 Polar(double r, double radians);
double PrincipleAngle(double radians);
#endif
