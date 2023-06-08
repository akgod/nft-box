
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <string>
#include <math.h>
#include <eosio/permission.hpp>
#include <eosio/crypto.hpp>


using namespace std;
using namespace eosio;
#define BBLB_CONTRACT "mboxexlbbnft"_n
#define BB_SYMBOL symbol("BB",4)
// #define BBLB_CONTRACT "boxbbbboxbbb"_n
// #define BB_SYMBOL symbol("BBT",4)

class [[eosio::contract("nft")]] nft : public eosio::contract {

  public:     
    using contract::contract;

    [[eosio::action]]     //id 默认为0，
    void setconfig(const uint64_t& id,const symbol_code& sym, const string& version,const uint64_t& token_id) {
        require_auth( get_self() );
            // valid symbol
        check( sym.is_valid(), "not valid symbol" );

        config_index config_table(get_self(), get_self().value);
        auto iterator = config_table.find(id);
        if( iterator == config_table.end() )
        {
            config_table.emplace(get_self(), [&]( auto& row ) {         //emplace( name payer, Lambda&& constructor ) 
            row.id = id;
            row.version = version;            
            row.symbol = sym;           
            row.token_id = 1;   
            });
        }
        else {
            config_table.modify(iterator, get_self(), [&]( auto& row ) {    //modify( const T& obj, name payer, Lambda&& updater )
            row.id = id;
            row.version = version;            
            row.symbol = sym;           
            row.token_id = 1;
            });
        }
    }

    [[eosio::action]]    //id默认设为1， data_id设为1
    void dataconfig(const uint64_t& id,const uint64_t& data_id) {
        require_auth( get_self() );

        dataid_index data_table(get_self(), get_self().value);
        auto iterator = data_table.find(id);
        if( iterator == data_table.end() )
        {
            data_table.emplace(get_self(), [&]( auto& row ) {         //emplace( name payer, Lambda&& constructor ) 
            row.id = id;        
            row.data_id = data_id;   
            });
        }
        else {
            data_table.modify(iterator, get_self(), [&]( auto& row ) {    //modify( const T& obj, name payer, Lambda&& updater )                     
            row.data_id = data_id;
            });
        }
    }
        

    [[eosio::action]]
    void create(uint64_t category_tag,name owner,uint64_t latest_price,uint64_t fomo_price) {

        require_auth( name("blindboxgafi") );                       
       
        
        // get token_id
        config_index config_table( get_self(), get_self().value );
        auto iterator = config_table.find(0);
        check(iterator != config_table.end(),"no config");
        auto nft_id = iterator->token_id;

        nft_index nft_table( get_self(), get_self().value );         
        nft_table.emplace( get_self(), [&]( auto& n ) {
            n.id = nft_id;
            n.category_tag = category_tag;
            n.owner = owner;            
            n.latest_price = latest_price;           
            n.fomo_price = fomo_price;
            n.fomo_times = 1;
        });

        config_table.modify(iterator, get_self(), [&]( auto& row ) {    //modify( const T& obj, name payer, Lambda&& updater )
           
            row.token_id++;
        });

    }


    [[eosio::action]]
    void burnnft(const uint64_t& nft_id,const uint64_t& category_tag,const uint64_t& type,const string& data) {        

        nft_index nft_table( get_self(), get_self().value );        
        auto itr = nft_table.find( nft_id);
        check(itr != nft_table.end(), "nft does not exist" );
        check(category_tag == itr->category_tag,"wrong category");
        require_auth(itr->owner);        
        

        dataid_index data_table(get_self(), get_self().value);
        auto iterator = data_table.find(1);
        check(iterator != data_table.end(),"no data_id in datatable");

        toipfs_index toipfs_table(get_self(), get_self().value);
        auto itr_toipfs = toipfs_table.find(itr->category_tag);
        check(itr_toipfs != toipfs_table.end(), "toipfs table have not id 13" ); 
        double ipfs_amount;
        if(type == 1){ipfs_amount = 0;}
        if(type == 2){ipfs_amount = itr_toipfs->amount;}
        
        swapdata_index swapdata_index_table( get_self(), get_self().value );         
        swapdata_index_table.emplace( get_self(), [&]( auto& n ) {
            n.id = iterator->data_id;
            n.category_tag = category_tag;
            n.type = type;
            n.owner = itr->owner; 
            n.ipfs_amount = ipfs_amount;           
            n.data = data;           
            n.swap_time = (uint64_t)current_time_point().sec_since_epoch();
        });
        
        data_table.modify(iterator, get_self(), [&]( auto& row ) {    //modify( const T& obj, name payer, Lambda&& updater )
           
            row.data_id++;
        });

        nft_table.erase( itr );     // erase itr

    }

    [[eosio::action]]
    void changeprice(const uint64_t& nft_id,const uint64_t& price) {

        nft_index nft_table( get_self(), get_self().value );        
        auto itr = nft_table.find( nft_id);
        check(itr != nft_table.end(), "nft_id does not exist" );
        require_auth(itr->owner);
        // change fomo price
        nft_table.modify(itr, get_self(), [&]( auto& row ) {    //modify( const T& obj, name payer, Lambda&& updater )
           
            row.fomo_price = price;
        });
            
    }


    [[eosio::action]]
    void transfernft(const name& from,const name& to,const uint64_t& nft_id,const string& memo ) {

        // ensure authorized to send from account
        check( from != to, "cannot transfer to self" );
        require_auth( from );
        // ensure 'to' account exists
        check( is_account( to ), "to account does not exist");
        // check memo size
        check( memo.size() <= 256, "memo has more than 256 bytes" );
        

        _changeowner( from, to, nft_id);
    }
//向本合约转BB,memo: “ipfs+data” memo.substr(0,4) == ipfs是BB兑换ipfs算力. 否则则是集合石交易；
    [[eosio::on_notify("mboxexlbbnft::transfer")]]
    void buynft(const name& from,const name& to,const asset& quantity,const string& memo) {
        // allow EOS to be sent by sending with empty string memo
        if ( memo == "deposit" ) return;
        if (memo =="") return;
        // don't allow spoofs
        if ( to != get_self() ) return;
        if ( from == name("eosio.stake") ) return;
        check( quantity.symbol == BB_SYMBOL, "Buy only with BB" );
        // check( memo.length() <= 256, "memo too long" );

//截取memo第一个空格前的字符
        if(memo.substr(0,4) == "ipfs"){   //判断是否是 ipfs算力兑换
            dataid_index data_table(get_self(), get_self().value);
            auto iterator = data_table.find(1);
            check(iterator != data_table.end(),"no data_id in datatable");

            toipfs_index toipfs_table(get_self(), get_self().value);
            auto itr_toipfs = toipfs_table.find(13);
            check(itr_toipfs != toipfs_table.end(), "toipfs table have not id 13" ); 


            swapdata_index swapdata_index_table( get_self(), get_self().value );         
            swapdata_index_table.emplace( get_self(), [&]( auto& n ) {
                n.id = iterator->data_id;
                n.category_tag = 88;
                n.type = 3;
                n.owner = from;            
                n.ipfs_amount = itr_toipfs->amount;
                n.data = memo.substr(4);     //从第5个位置开始至结尾      
                n.swap_time = (uint64_t)current_time_point().sec_since_epoch();
            });
            
            data_table.modify(iterator, get_self(), [&]( auto& row ) {    //modify( const T& obj, name payer, Lambda&& updater )
            
                row.data_id++;
            });
            return;
        }

        uint64_t nft_id =(uint64_t)stoull(memo);   //需将string转为 uint64_t
        //把币转给 nft的原主人，更新latest_price,fomo_price ,更新owner    

        nft_index nft_table( get_self(), get_self().value ); 
        auto itr = nft_table.find( nft_id);
        check(itr != nft_table.end(), "nft_id does not exist" ); 
        check(itr->fomo_price == quantity.amount,"你的出价不合理，再试一次");      

        nft_table.modify(itr, get_self(), [&]( auto& row ) {    //modify( const T& obj, name payer, Lambda&& updater )
            row.latest_price = quantity.amount;
            row.fomo_price = (uint64_t)(itr->latest_price*1.05 + 0.5);
            row.fomo_times = itr->fomo_times +1;
        });

        uint64_t BB_amount = (uint64_t)(quantity.amount*0.99 + 0.5);   //平台收取1%的手续费

        action(
            permission_level{ get_self(), name("active") },
            BBLB_CONTRACT, name("transfer"),
            std::make_tuple(get_self(), itr->owner,asset(BB_amount,BB_SYMBOL), std::string("你的矿石已经被人强行买走"))
        ).send();

        _changeowner( itr->owner, from, nft_id); 


    }

    [[eosio::action]]
    void updateipfs(){      //每分钟更新一次 ipfs的兑换汇率
        require_auth( name("blindboxgafi") );
        pair_index pairs("swap.defi"_n,"swap.defi"_n.value);
        auto itr_eosusdt = pairs.find(12);
        check(itr_eosusdt != pairs.end(),"defibox pairs have no id 12");
        auto eosprice = itr_eosusdt->price0_last;

        auto itr_bbeos = pairs.find(831);
        check(itr_bbeos != pairs.end(),"defibox pairs have no id 831");
        auto bbprice = (itr_bbeos->reserve0.amount*0.0001)/(itr_bbeos->reserve1.amount*0.0001);

        auto bb_to_ipfs = bbprice * eosprice * 6.5/3500;  //一个BB可兑换 ipfs的数量，以T计
        // action(
        //     permission_level{ get_self(), name("active") },
        //     get_self(), name("log"),
        //     std::make_tuple(eosprice,bbprice,bb_to_ipfs,(uint64_t)(1*bb_to_ipfs*1000+0.5)/1000)
        // ).send();
        for(auto i =1;i<14;i++){
            double amount;
            if(i==1){amount = ((uint64_t)(1*bb_to_ipfs*1000+0.5))*0.001;}
            if(i==2){amount = ((uint64_t)(2*bb_to_ipfs*1000+0.5))*0.001;}
            if(i==3){amount = ((uint64_t)(3*bb_to_ipfs*1000+0.5))*0.001;}
            if(i==4){amount = ((uint64_t)(5*bb_to_ipfs*1000+0.5))*0.001;}
            if(i==5){amount = ((uint64_t)(6*bb_to_ipfs*1000+0.5))*0.001;}
            if(i==6){amount = ((uint64_t)(7*bb_to_ipfs*1000+0.5))*0.001;}
            if(i==7){amount = ((uint64_t)(20*bb_to_ipfs*1000+0.5))*0.001;}
            if(i==8){amount = ((uint64_t)(25*bb_to_ipfs*1000+0.5))*0.001;}
            if(i==9){amount = ((uint64_t)(30*bb_to_ipfs*1000+0.5))*0.001;}
            if(i==10){amount = ((uint64_t)(50*bb_to_ipfs*1000+0.5))*0.001;}
            if(i==11){amount = ((uint64_t)(60*bb_to_ipfs*1000+0.5))*0.001;}
            if(i==12){amount = ((uint64_t)(70*bb_to_ipfs*1000+0.5))*0.001;}
            if(i==13){amount = ((uint64_t)(1*bb_to_ipfs*1000+0.5))*0.001;}
            toipfs_index toipfs_table(get_self(), get_self().value);
            auto iterator = toipfs_table.find(i);
            if( iterator == toipfs_table.end() )
            {
                toipfs_table.emplace(get_self(), [&]( auto& row ) {         //emplace( name payer, Lambda&& constructor ) 
                row.category_tag = i;        
                row.amount = amount;   
                });
            }
            else {
                toipfs_table.modify(iterator, get_self(), [&]( auto& row ) {    //modify( const T& obj, name payer, Lambda&& updater )                     
                row.amount = amount;
                });
            }
        }



    }
    [[eosio::action]]
    void log(double eosprice,double bbprice,double bb_to_ipfs ,double amount){  }


    [[eosio::action]]
    void eraseconfig(uint64_t id) {
      require_auth(get_self());
      config_index config_table(get_self(), get_self().value);
      auto iterator = config_table.find(id);

      check(iterator != config_table.end(), "Record does not exist");
      config_table.erase(iterator);
    }

    [[eosio::action]]
    void erasenftdata(uint64_t nft_id) {
      require_auth(get_self());

      nft_index nft_table( get_self(), get_self().value ); 
      auto itr = nft_table.find( nft_id);
      check(itr != nft_table.end(), "Record does not exist");
      nft_table.erase(itr);
    }
    [[eosio::action]]
    void eraseallnft() {
      require_auth(get_self());

      nft_index nft_table( get_self(), get_self().value );      
      for (auto itr = nft_table.begin(); itr != nft_table.end();){         // erase table,***
        itr = nft_table.erase(itr);
      }

    }
    [[eosio::action]]
    void deleswapdata() {
      require_auth(get_self());

      swapdata_index swapdataid_table( get_self(), get_self().value );      
      for (auto itr = swapdataid_table.begin(); itr != swapdataid_table.end();){         // erase table,***
        itr = swapdataid_table.erase(itr);
      }

    }

  private:
    
    void _changeowner(const name& from, const name& to, const uint64_t& nft_id) {

        nft_index nft_table( get_self(), get_self().value );        
        const auto& itr = nft_table.find( nft_id);
        check(itr != nft_table.end(), "nft does not exist" );
        //check( itr->owner == from, "must be nft owner" );           

        // notifiy both parties
        require_recipient( from );
        require_recipient( to );
        nft_table.modify( itr, get_self(), [&] (auto& t ) {
            t.owner = to;
        });             
    }

    struct [[eosio::table]] tokenconfig {  //用于存nft的id,++
        uint64_t id;
        string version;
        symbol_code symbol;
        uint64_t token_id;
        uint64_t primary_key() const { return id; }

    };
    using config_index =  eosio::multi_index< "tokenconfigs"_n, tokenconfig >;


    struct [[eosio::table]] dataid {   //用于存swapdata的用户信息id,++   默认id=1
        uint64_t id;
        uint64_t data_id;
        uint64_t primary_key() const { return id; }

    };
    using dataid_index =  eosio::multi_index< "dataids"_n, dataid >;


    struct [[eosio::table]] swapdata {
      uint64_t id;
      uint64_t category_tag;
      name owner;
      uint64_t type;   // 1-实物兑换，2 - 矿石兑换ipfs算力 3 - BB兑换ipfs算力
      std::string data;
      double ipfs_amount;
      uint64_t swap_time;
      uint64_t primary_key() const { return id; }
      uint64_t get_secondary_1() const { return swap_time; }
    };
    using swapdata_index = eosio::multi_index<"swapdatas"_n,swapdata,
                                    indexed_by<"bytime"_n, const_mem_fun<swapdata, uint64_t, &swapdata::get_secondary_1>>
                                    >; 

    struct [[eosio::table]] toipfs {
        uint64_t category_tag;
        double amount;
        uint64_t primary_key() const { return category_tag; }

    };
    using toipfs_index =  eosio::multi_index< "toipfss"_n, toipfs >;

  //deifbox的交易对表

    struct token {
	    name   contrat_name;
        symbol token_symbol;
    };

    struct [[eosio::table]] pair {
      uint64_t id;
      token token0;
      token token1;
      asset reserve0;
      asset reserve1;
      uint64_t liquidity_token;
      double price0_last;
      double price1_last;
      uint64_t price0_cumulative_last;
      uint64_t price1_cumulative_last;
      time_point_sec block_time_last;
      uint64_t primary_key() const { return id; }
    };
    using pair_index = eosio::multi_index<"pairs"_n, pair>;


    struct [[eosio::table]] nftdata {
        uint64_t id;
        uint64_t category_tag;
        name owner;
        uint64_t latest_price;
        uint64_t fomo_price;
        uint64_t fomo_times;

        uint64_t primary_key() const { return id; }
        uint64_t get_secondary_1() const { return category_tag; }
        uint64_t get_secondary_2() const { return owner.value; }
        uint64_t get_secondary_3() const { return fomo_times; }
        
    };

    using nft_index =  eosio::multi_index< "nftdatas"_n, nftdata, 
                                indexed_by<"bycategory"_n, const_mem_fun<nftdata, uint64_t, &nftdata::get_secondary_1>>,
                                indexed_by<"byowner"_n, const_mem_fun<nftdata, uint64_t, &nftdata::get_secondary_2>>,
                                indexed_by<"byfomotimes"_n, const_mem_fun<nftdata, uint64_t, &nftdata::get_secondary_3>>
                              >;

  };


