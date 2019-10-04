/*
Смарт-контракт “KittyShelter” должен взаимодействовать с одним “KittenRegistry” по указанному адресу (адрес можно захардкодить или указать в конструкторе).

У смарт-контракта должно быть две (обязательных) функции с такой сигнатурой:

function storeKitty(uint256 catId, uint256 time) public
function retrieveKitty(uint256 catId) public

Один пользователь может оставить неограниченное количество котов в  “KittyShelter”.

Пользователь не может забрать кота раньше срока.

Кот остается у смарт-контракта до тех пор, пока его не заберут. Пользователь должен сам “прийти” за котом, вызвав функцию retrieveKitty.

Забрать кота может только тот пользователь, который его отдал.

Результатом проделанной работы будет исходный код смарт-контракта “KittyShelter”. Как бонус, можно предоставить адрес рабочего контракта в тестовой сети Ropsten.
*/
contract KittyShelter{
 
   KittenRegistry registry;
  
   address owner;
   struct info_cats{
       
        uint256 id_cat;
        uint256 end_date;
       
   } 
  
  
   struct info{
   
        bool is_exist;
        info_cats[] cats;
   
   }
    
   
    
   mapping(address => info) humans_;
 
 
    constructor()public{
      
       owner = msg.sender;
       
       
    }
    
 



    function find_cat(uint256 id_cat, address human) private view returns(int256){
        
        require(humans_[human].is_exist != false, "Can't find user!");
        
        for(uint256 i = 0; i < humans_[human].cats.length; ++i){
            
            if (humans_[human].cats[i].id_cat == id_cat){
                
                return int256(i);
                
            }
            
        }
        
        return -1;
        
        
    }

    function is_find_cat(uint256 id_cat, address human) private view returns(bool){
        
        return find_cat(id_cat,human) != -1;
        
        
    }
    
    
    function remove_cat(uint256 id_cat, address human) private{
        
          for(uint256 i = 0; i < humans_[human].cats.length; ++i){
            
            if (humans_[human].cats[i].id_cat == id_cat){
                
                delete humans_[human].cats[i];
                
                
            }
            
        }
       
        
        require(true,"Can't find cat(remove)!");
    
    }
    
    function storeKitty(uint256 id_cat, uint256 time) public{
        
        require(registry.getApproved(id_cat) == address(this));
      
        registry.transferFrom(msg.sender,address(this),id_cat);
        
        info_cats memory  tmp;
        tmp.id_cat = id_cat;
        tmp.end_date = now + convert_date_to_day(time);
        humans_[msg.sender].cats.push(tmp);
        humans_[msg.sender].is_exist = true;
     
    }
    
    function retrieveKitty(uint256 catId) public{
        
        require(registry.ownerOf(catId) == address(this));
        require(humans_[msg.sender].is_exist,"Can't find user!");
        int256 index = find_cat(catId,msg.sender);
        require(index != -1,"It is not your cat!");
        require(humans_[msg.sender].cats[uint256(index)].end_date < now, "The time has not come yet, goodbye!");
        
        registry.transferFrom(address(this),msg.sender,catId);
        
        remove_cat(catId,msg.sender);
        
        
    }
    
    function setRegistry(KittenRegistry registry_) public{
        
        require(owner == msg.sender);
        
        registry = registry_;
        
    }
    
    
    
 
    function convert_date_to_day(uint256 date) private pure returns(uint256){
     
          return date * 86400;
     
    }
   
 
  
    
}