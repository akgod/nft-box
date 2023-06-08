#include <string>
#include <eosio/asset.hpp>
#include "RandomSeed.hpp"


using namespace eosio;
using namespace std;
#define EOS_CONTRACT "eosio.token"_n
#define EOS_SYMBOL symbol("EOS",4)
#define BBLB_CONTRACT "mboxexlbbnft"_n
#define NFT_CONTRACT "luckyballeos"_n
#define LB_SYMBOL symbol("LB",4)
#define BB_SYMBOL symbol("BB",4)

// #define BBLB_CONTRACT "boxbbbboxbbb"_n
// #define NFT_CONTRACT "51app51app51"_n
// #define LB_SYMBOL symbol("LBT",4)
// #define BB_SYMBOL symbol("BBT",4)

class [[eosio::contract("go")]] go : public eosio::contract {

  public:
    using contract::contract;

    [[eosio::on_notify("eosio.token::transfer")]]
    void eosplay(const name& from, const name& to,const asset& quantity, const string& memo) {
      
          if (from == get_self() || to != get_self() || from =="eosio.ram"_n||from =="eosio.stake"_n){return;}
          if(quantity.symbol != EOS_SYMBOL ){return;}  
          if(memo == "deposit"){return;}    
            
          check(quantity.is_valid(), "Invalid token transfer");      
          

          //probability set
          uint64_t rock_under = 0; 
          uint64_t dmt_one = 0;
          uint64_t dmt_two = 0;
          uint64_t dmt_three = 0;
          uint64_t category =0;
                                               
          if(quantity.amount == 10000  ){category=(uint64_t)1;rock_under=(uint64_t)70;dmt_one=90;dmt_two=(uint64_t)8;dmt_three=(uint64_t)2;}        //开到盒子的概率70%，开到nft1-90%，nft2-8%,nft3-2%
          if(quantity.amount == 50000  ){category=(uint64_t)2;rock_under=(uint64_t)80;dmt_one=90;dmt_two=(uint64_t)8;dmt_three=(uint64_t)2;}       
          if(quantity.amount == 200000 ){category=(uint64_t)3;rock_under=(uint64_t)90;dmt_one=90;dmt_two=(uint64_t)8;dmt_three=(uint64_t)2;}
          if(quantity.amount == 500000  ){category=(uint64_t)4;rock_under=(uint64_t)100;dmt_one=95;dmt_two=(uint64_t)3;dmt_three=(uint64_t)2;}
       
          if(category == 0){return;} //判断金额是否对

          //get inviter

          name inviter1 = name("macprobtc555");
          const name possible_inviter = name(memo);                
            
          if (possible_inviter != get_self() && possible_inviter != from && is_account(possible_inviter)){
            inviter1 = possible_inviter;
          }


          name inviter2 = name("dbeostokenio");
          player_index players(get_self(), get_self().value);
          auto itr = players.find(inviter1.value);
          if(itr != players.end()){
            if(itr->inviter1.value ){
              inviter2 = itr->inviter1;     
            }    
          }
      
          
          auto p = players.find(from.value);
          if( p == players.end()){                    //not found,new player
            p = players.emplace(get_self(), [&](auto& item){
              item.player = from;
              item.inviter1 = inviter1;
              item.inviter2 = inviter2;
              item.myivttimes = 0;
            });
            
            auto mts = players.find(inviter1.value);                 
            if( mts != players.end()){                //the inviter->myivttimes+1.
              players.modify(mts, get_self(), [&](auto& item){
              item.myivttimes += 1;
              });    
            }          
          }

        //go
        bet(from, quantity,category, rock_under, dmt_one, dmt_two, dmt_three);       

    }

    [[eosio::on_notify("mboxexlbbnft::transfer")]]
    void lbpaly(const name& from, const name& to,const asset& quantity, const string& memo) {
      
          if (from == get_self() || to != get_self() || from =="eosio.ram"_n||from =="eosio.stake"_n){return;}
          if( quantity.symbol != LB_SYMBOL){return;}  
          if(memo == "deposit"){return;}    
          check(quantity.is_valid(), "Invalid token transfer");     
          

          //probability set
          uint64_t rock_under = 0; 
          uint64_t dmt_one = 0;
          uint64_t dmt_two = 0;
          uint64_t dmt_three = 0;
          uint64_t category =0;
                                              
          if(quantity.amount == 100000  ){category=(uint64_t)1;rock_under=(uint64_t)60;dmt_one=90;dmt_two=(uint64_t)8;dmt_three=(uint64_t)2;}        //开到盒子的概率70%，开到nft1-90%，nft2-8%,nft3-2%
          if(quantity.amount == 500000  ){category=(uint64_t)2;rock_under=(uint64_t)80;dmt_one=90;dmt_two=(uint64_t)8;dmt_three=(uint64_t)2;}       
          if(quantity.amount == 2000000 ){category=(uint64_t)3;rock_under=(uint64_t)90;dmt_one=90;dmt_two=(uint64_t)8;dmt_three=(uint64_t)2;}
          if(quantity.amount == 5000000  ){category=(uint64_t)4;rock_under=(uint64_t)100;dmt_one=95;dmt_two=(uint64_t)3;dmt_three=(uint64_t)2;}
         
          if(category == 0){return;} //判断金额是否对

          //get inviter

          name inviter1 = name("macprobtc555");
          const name possible_inviter = name(memo);                
            
          if (possible_inviter != get_self() && possible_inviter != from && is_account(possible_inviter)){
            inviter1 = possible_inviter;
          }
        //players---inviter table update


          name inviter2 = name("dbeostokenio");
          player_index players(get_self(), get_self().value);
          auto itr = players.find(inviter1.value);
          if(itr != players.end()){
            if(itr->inviter1.value ){
              inviter2 = itr->inviter1;     
            }    
          }
      
          
          auto p = players.find(from.value);
          if( p == players.end()){                    //not found,new player
            p = players.emplace(get_self(), [&](auto& item){
              item.player = from;
              item.inviter1 = inviter1;
              item.inviter2 = inviter2;
              item.myivttimes = 0;
            });
            
            auto mts = players.find(inviter1.value);                 
            if( mts != players.end()){                //the inviter->myivttimes+1.
              players.modify(mts, get_self(), [&](auto& item){
              item.myivttimes += 1;
              });    
            }          
          }

        //go
        bet(from, quantity,category, rock_under, dmt_one, dmt_two, dmt_three);      

    }



    [[eosio::action]]
    void erasetab(uint64_t tab) {
      require_auth(_self);

      if(tab == 0){
          result_index result_table(get_self(), get_self().value);
          for (auto clean = result_table.begin(); clean != result_table.end();){         // erase table,清空result
            clean = result_table.erase(clean);
          } 
      }     

    }
    [[eosio::action]]
    void eraseresult(name player) {
      require_auth(_self); 
      result_index result_table(get_self(), get_self().value);   
      auto iterator = result_table.find(player.value);      // erase table,***
      check(iterator != result_table.end(), "Record does not exist");
      result_table.erase(iterator);         

    }

    [[eosio::action]]
    void log(std::string rock_under,std::string random_number,std::string random_dmtnum){
      require_auth(get_self());
    }


    [[eosio::action]]
    void updatebbhold(){     //每天12点更新一次,BB富豪榜
      require_auth(get_self());
      player_index players( get_self(), get_first_receiver().value );
      bbhold_index bbholds( get_self(), get_first_receiver().value );
      for(auto itr = players.begin();itr != players.end();itr++){
          uint64_t bb_balance = get_balance( name("mboxexlbbnft"), itr->player, BB_SYMBOL );
          auto itr_bb = bbholds.find(itr->player.value);

          if( itr_bb == bbholds.end() ){
              bbholds.emplace(get_self(), [&]( auto& row ) {         //emplace( name payer, Lambda&& constructor ) 
                row.player = itr->player;
                row.balance = bb_balance;         
              });
          }else {
              bbholds.modify(itr_bb, get_self(), [&]( auto& row ) {    //modify( const T& obj, name payer, Lambda&& updater )                
                row.balance = bb_balance;            
              });
          }     
      }
    }

  private:
    void bet(name player, asset quantity,uint64_t category, uint64_t rock_under ,uint64_t dmt_one,uint64_t dmt_two,uint64_t dmt_three ) {

      check(quantity.amount > 0, "must bet a positive amount--error amount");
      check(quantity.symbol == EOS_SYMBOL || quantity.symbol == LB_SYMBOL, "only EOS/LB allowed" );      
 
      //generate bet_id
      auto s = read_transaction(nullptr, 0);
      char *tx = (char *)malloc(s);
      read_transaction(tx, s);
      checksum256 tx_hash = sha256(tx, s);

      const uint64_t bet_id = ((uint64_t)tx_hash.data()[0] << 56) + ((uint64_t)tx_hash.data()[1] << 48) + ((uint64_t)tx_hash.data()[2] << 40) + ((uint64_t)tx_hash.data()[3] << 32) + ((uint64_t)tx_hash.data()[4] << 24) + ((uint64_t)tx_hash.data()[5] << 16) + ((uint64_t)tx_hash.data()[6] << 8) + (uint64_t)tx_hash.data()[7];           
        
        
      uint64_t random_number = (uint64_t) random_gen::get_instance(player,bet_id).range(100);     //random number 0-100 
      uint64_t BB_amount = 0;
      uint64_t LB_amount =0;
      if (category == 1){
        BB_amount = (uint64_t) random_gen::get_instance(player,bet_id).range(10000);
        LB_amount = (uint64_t) random_gen::get_instance(player,bet_id).range(20000);
      }
      if (category == 2){
        BB_amount = (uint64_t) random_gen::get_instance(player,bet_id).range(50000);
        LB_amount = (uint64_t) random_gen::get_instance(player,bet_id).range(100000);
      }
      if (category == 3){
        BB_amount = (uint64_t) random_gen::get_instance(player,bet_id).range(200000);
        LB_amount = (uint64_t) random_gen::get_instance(player,bet_id).range(400000);
      }
      if (category == 4){
        BB_amount = (uint64_t) random_gen::get_instance(player,bet_id).range(500000);
        LB_amount = (uint64_t) random_gen::get_instance(player,bet_id).range(1000000);
      }

      uint64_t bonus_inv1 = (uint64_t)(LB_amount * 0.05 +1);
      uint64_t bonus_inv2 = (uint64_t)(LB_amount * 0.02 +1);
      
      //airdrop BB & LB to player no matter win or lose
      action(
          permission_level{get_self(), name("active")},
          BBLB_CONTRACT, name("transfer"),
          make_tuple(get_self(), player, asset(BB_amount, BB_SYMBOL), std::string("Mboxex 开盒奖励BB "))
      ).send();
      action(
          permission_level{get_self(), name("active")},
          BBLB_CONTRACT, name("transfer"),
          make_tuple(get_self(), player, asset(LB_amount, LB_SYMBOL), std::string("Mboxex 开盒奖励LB "))
      ).send();

      //send bonus_inv to the inviter1 and inviter2 no matter win or lose
      player_index players(get_self(), get_self().value);
      auto q = players.find(player.value);
      if( q != players.end()){   
          if(q->inviter1 != name("macprobtc555")) {
            action(
                permission_level{get_self(), name("active")},
                BBLB_CONTRACT, name("transfer"),
                make_tuple(get_self(), player, asset(bonus_inv1, LB_SYMBOL), std::string("Mboxex 好友邀请奖励加成LB"))
            ).send();
          }    
                                 
          action(
            permission_level{get_self(), name("active")},
            BBLB_CONTRACT, name("transfer"),
            make_tuple(get_self(), q->inviter1, asset(bonus_inv1, LB_SYMBOL), std::string("Mboxex 邀请奖励LB "))
          ).send();

          action(
            permission_level{get_self(), name("active")},
            BBLB_CONTRACT, name("transfer"),
            make_tuple(get_self(), q->inviter2, asset(bonus_inv2, LB_SYMBOL), std::string("Mboxex 邀请奖励LB "))
          ).send();

      } 
        
      if(random_number <= rock_under){             //player win   ,create NFT，
          //generate ramdom_dmtid
          auto s = read_transaction(nullptr, 0);
          char *tx = (char *)malloc(s);
          read_transaction(tx, s);
          checksum256 tx_hash = sha256(tx, s);
          
          const uint64_t ramdom_dmtid = ((uint64_t)tx_hash.data()[0] << 58) + ((uint64_t)tx_hash.data()[1] << 46) + ((uint64_t)tx_hash.data()[2] << 39) + ((uint64_t)tx_hash.data()[3] << 28) + ((uint64_t)tx_hash.data()[4] << 22) + ((uint64_t)tx_hash.data()[5] << 16) + ((uint64_t)tx_hash.data()[6] << 8) + (uint64_t)tx_hash.data()[7];           
            
            
          uint64_t random_dmtnum = (uint64_t) random_gen::get_instance(player,ramdom_dmtid).range(100);     //random number 0-100 


         // check(false,"random_dmtnum=" + std::to_string(random_dmtnum) + "rock_under=" + std::to_string(rock_under) + "random_number="+ std::to_string(random_number));
          action(
            permission_level{get_self(), name("active")},
            get_self(), name("log"),
            make_tuple(std::to_string(rock_under),std::to_string(random_number),std::to_string(random_dmtnum))
          ).send();

          uint64_t category_tag;
          uint64_t latest_price;
          uint64_t fomo_price;
          if(category ==1){
            if(0<=random_dmtnum && random_dmtnum<=dmt_one){category_tag=1;latest_price=10000;fomo_price=10000;}
            if(dmt_one < random_dmtnum && random_dmtnum<= dmt_one+dmt_two){category_tag=2;latest_price=20000;fomo_price=20000;}
            if(dmt_one+dmt_two <random_dmtnum && random_dmtnum <=100){category_tag=3;latest_price=30000;fomo_price=30000;}
          }
          if(category ==2){
            if(0<=random_dmtnum && random_dmtnum <=dmt_one){category_tag=4;latest_price=50000;fomo_price=50000;}
            if(dmt_one < random_dmtnum && random_dmtnum <= dmt_one+dmt_two){category_tag=5;latest_price=60000;fomo_price=60000;}
            if(dmt_one+dmt_two <random_dmtnum && random_dmtnum <=100){category_tag=6;latest_price=70000;fomo_price=70000;}
          }
          if(category ==3){
            if(0<=random_dmtnum && random_dmtnum <=dmt_one){category_tag=7;latest_price=200000;fomo_price=200000;}
            if(dmt_one < random_dmtnum && random_dmtnum <= dmt_one+dmt_two){category_tag=8;latest_price=250000;fomo_price=250000;}
            if(dmt_one+dmt_two <random_dmtnum && random_dmtnum <=100){category_tag=9;latest_price=300000;fomo_price=300000;}
          }
          if(category ==4){
            if(0<=random_dmtnum && random_dmtnum<=dmt_one){category_tag=10;latest_price=500000;fomo_price=500000;}
            if(dmt_one < random_dmtnum && random_dmtnum<= dmt_one+dmt_two){category_tag=11;latest_price=600000;fomo_price=600000;}
            if(dmt_one+dmt_two <random_dmtnum && random_dmtnum <=100){category_tag=12;latest_price=700000;fomo_price=700000;}
          }
          
          result_index result_table(get_self(), get_self().value);
          auto iterator = result_table.find(player.value);
          if( iterator == result_table.end() )
          {
              result_table.emplace(get_self(), [&]( auto& row ) {         //emplace( name payer, Lambda&& constructor ) 
                row.player = player;
                row.category_tag = category_tag;         
                row.BB_amount = BB_amount;
                row.LB_amount = LB_amount;
                row.result_time = (uint64_t)current_time_point().sec_since_epoch();
              });
          }
          else {
              result_table.modify(iterator, get_self(), [&]( auto& row ) {    //modify( const T& obj, name payer, Lambda&& updater )                
                row.category_tag = category_tag; 
                row.BB_amount = BB_amount;
                row.LB_amount = LB_amount;
                row.result_time = (uint64_t)current_time_point().sec_since_epoch();           
              });
          }

          action(
              permission_level{get_self(), name("active")},
              NFT_CONTRACT, name("create"),
              make_tuple(category_tag,player,latest_price,fomo_price)
          ).send();              

      }else{
          uint64_t category_tag = 0;
          result_index result_table(get_self(), get_self().value);
          auto iterator = result_table.find(player.value);
          if( iterator == result_table.end() )
          {
              result_table.emplace(get_self(), [&]( auto& row ) {         //emplace( name payer, Lambda&& constructor ) 
                row.player = player;
                row.category_tag = category_tag;   
                row.BB_amount = BB_amount;
                row.LB_amount = LB_amount;
                row.result_time = (uint64_t)current_time_point().sec_since_epoch();      
              });
          }
          else {
              result_table.modify(iterator, get_self(), [&]( auto& row ) {    //modify( const T& obj, name payer, Lambda&& updater )                
                row.category_tag = category_tag; 
                row.BB_amount = BB_amount;
                row.LB_amount = LB_amount;
                row.result_time = (uint64_t)current_time_point().sec_since_epoch();           
              });
          }
      }
    }


    //玩家表
    struct [[eosio::table]] player {
      name player;
      name inviter1;
      name inviter2;
      uint64_t myivttimes; 
      uint64_t primary_key() const { return player.value; }
    };
    using player_index = eosio::multi_index<"players"_n,player>;  

    //开奖结果表
    struct [[eosio::table]] result {
      name player;
      uint64_t category_tag;   // 0- 谢谢参与，1-12对应 相应石头
      uint64_t BB_amount;
      uint64_t LB_amount;
      uint64_t result_time;
      uint64_t primary_key() const { return player.value; }
    };
    using result_index = eosio::multi_index<"results"_n,result>; 

    //BB持仓量
    struct [[eosio::table]] bbhold {
      name player;
      uint64_t balance;   // 该用户的BB 余额
      uint64_t primary_key() const { return player.value; }
      uint64_t get_secondary_1() const { return balance; }
    };
    using bbhold_index = eosio::multi_index<"bbholds"_n,bbhold,
                                  indexed_by<"bybalance"_n, const_mem_fun<bbhold, uint64_t, &bbhold::get_secondary_1>>
                                >;

    struct [[eosio::table]] account {
        asset    balance;

        uint64_t primary_key()const { return balance.symbol.code().raw(); }
    };

    using accounts_index = eosio::multi_index< "accounts"_n, account > ;

    uint64_t get_balance( const name& token_contract_account, const name& owner, const symbol& sym ){
        accounts_index accountstable( token_contract_account, owner.value );
        const auto& ac = accountstable.get( sym.code().raw() );
        return ac.balance.amount;
    }

};





