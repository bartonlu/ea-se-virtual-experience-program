#ifndef GIFTING_RED_POCKETS_H
#define GIFTING_RED_POCKETS_H

#include <string>
#include <map>
#include <vector>

using namespace std;

class Sim;
class GiftManager;

// Gift factory classes
class GiftFactory;
class RedPocketGiftFactory;
class OtherGiftFactory;
class Gift;
class RedPocketGift;
class OtherGift;

// Animation classes
class Animation;
class GiftAnimation;
class RedPocketGiftAnimation;
class GiftAnimationAdapter;

// Observer/subscriber classes
class Subscriber;
class RedPocketsSubscriber;

// Death Strategy classes
class KillSimStrategy;
class DeathStrategy;
class RedPocketDeathStrategy;
class OtherDeathStrategy;

/***********************/

class Sim {
    private:
        int id;
        string name;
        bool alive;
        vector<Subscriber*> subscribers;

    public:
        Sim(int id, const string& name);
        ~Sim() = default;

        void act();

        // Subscriber methods
        void addReceivedGift(Gift* gift);
        void subscribe(Subscriber* subscriber);
        void unsubscribe(Subscriber* subscriber);
        void notifySubscribers();

        int getId() const;
        const string& getName() const;
        bool isAlive() const;
};

class GiftManager {
    private:
        GiftFactory* giftFactory;
        Gift* gift;
        GiftAnimationAdapter* giftAnimationAdapter;
        Sim* sim;

    public:
        GiftManager(GiftFactory* giftFactory);
        void giveGift(Sim* gifter, Sim* recipient, const string& giftType, int value);
};

// Gift factory classes

class GiftFactory {
    private:
        Sim* recipient;
        Sim* gifter;
        map<string, int> giftTypeMappings;

    public:
        GiftFactory();
        virtual ~GiftFactory();
        virtual Gift* createGift();
};

class RedPocketGiftFactory : public GiftFactory {
    private:
        int value;
    
    public:
        RedPocketGiftFactory(int value);
        Gift* createGift() override;
        int getValue() const;
};

class OtherGiftFactory : public GiftFactory {
    public:
        Gift* createGift() override;
};

class Gift {
    public:
        virtual ~Gift();
        virtual void giveGift() = 0;
};

class RedPocketGift : public Gift {
    private:
        int value;
    
    public:
        RedPocketGift(int value);
        void giveGift() override;
        int getValue() const;
};

class OtherGift : public Gift {
    public:
        void giveGift() override;
};

// Animation classes

class Animation {
    public:
        virtual ~Animation() = default;
        virtual void play() = 0;
};

class GiftAnimation : public Animation {
    public:
        virtual void play() = 0;
};

class RedPocketGiftAnimation : public GiftAnimation {
    private:
        /* Currently, I am not sure what this class would need
        that is different from its parents.

        It may be that upon implementation of this feature, 
        we will find that this class is not needed. However, I
        feel that for now, it is a good class to keep in the
        system design, as there will likely be some properties
        and methods specific to the Red Pocket animation */
};

class GiftAnimationAdapter : public Gift {
    private:
        GiftAnimation* animation;

    public:
        GiftAnimationAdapter(Animation* animation);
        void giftGift();
};

// Observer/subscriber classes

class Subscriber {
    public:
        virtual ~Subscriber() = default;
        virtual void update(Sim* context) = 0;
};

class RedPocketSubscriber : public Subscriber {
    private:
        int counter;
    
    public:
        void update(Sim* context) override;
};

// Death strategy classes

class KillSimContext {
    private:
        DeathStrategy* deathStrategy;
        
    public:
        KillSimContext(DeathStrategy* deathStrategy);
        void executeDeath(Sim* sim);
};

class DeathStrategy {
    public:
        virtual ~DeathStrategy() = default;
        virtual void executeDeath(Sim* sim) = 0;
};

class RedPocketDeathStrategy : public DeathStrategy {
    public:
        void executeDeath(Sim* sim) override;
};

class OtherDeathStrategy : public DeathStrategy {
    public:
        void executeDeath(Sim* sim) override;
};

#endif
