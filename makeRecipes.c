#include <stdio.h>
#include <stdlib.h>
#include "app.h"

void makeRecipes (ingredients *fridgeContents, Recipes *vegetarianRecipes, Recipes *lowCarbRecipes, Recipes *lowCalorieRecipes, Recipes *highProteinRecipes, Recipes *allRecipes){
/*Declaration of different recipes*/
    Recipes falafel =  {"Falafel",

                     {{"Broccoli", 210}, {"Cauliflower", 130}, {"Carrots", 165}, {"Celery", }}, 

                     {{"Elbow_macaroni", 150}, {"Baking_soda", 3}, {"Cilantro_leaves", 17}, {"Mint_leaves", 15}, 
                     {"Garlic_cloves", 20}, {"Ground_coriander", 6}, {"Chili_powder", 6}, {"Salt", 6}, {"Pepper", 3}, {"Sesame_seeds", 6}, {"Baking_powder", 6}},

                     "db/recipes/pizza/instructions.txt",
                     
                     "Vegitarian",
                     
                     "4 pieces: 224 calories, 13g fat (1g saturated fat), 0 cholesterol, 760mg sodium, 32g carbohydrate (1g sugars, 16g fiber), 9g protein."};


    Recipes macNCheese =  {"MacNCheese",

                     {},

                     {{"Dried_Chickpeas", 170}, {"Baking_soda", 3}, {"Cilantro_leaves", 17}, {"Mint_leaves", 15}, 
                     {"Garlic_cloves", 20}, {"Ground_coriander", 6}, {"Chili_powder", 6}, {"Salt", 6}, {"Pepper", 3}, {"Sesame_seeds", 6}, {"Baking_powder", 6}},

                     "db/recipes/pizza/instructions.txt",
                     
                     "Vegitarian",
                     
                     "4 pieces: 224 calories, 13g fat (1g saturated fat), 0 cholesterol, 760mg sodium, 32g carbohydrate (1g sugars, 16g fiber), 9g protein."};

    Recipes jambalaya = {"Jambalaya",

                        {{"Diced_tomatoes", 793.8}, {"Reduced-sodium_chicken_broth", 240}, {"Green_pepper", 164},
                        {"Celery_ribs", 110}, {"White_wine", 117.2}, {"Boneless_chicken_thigh", 907.2},
                        {"Spicy_chicken_sausage", 340.2}, {"Medium_shrimp", 907.2}, {}},

                        {{"Onion", 170}, {"Garlic_cloves", 11.2}, {"Cajun_seasosning", 8}, {"Dried_parsley", 1.58},
                        {"Dried_basil", 0.71}, {"Dried_oregano", 1.01}, {"Salt", 4.5}, {"Cayenne_pepper", 1.8},
                        {"Brown_rice", 1520}},

                        "db/recipes/Jambalaya/instructions.txt",

                        "High protein"}

    Recipes chickenTikkaMasala = {"Chicken Tikka Masala",
    
                                {{"Tomato_puree", 425.24}, {"Tomato_paste", 28.1}, {"Grated_lemon_zest", 4.2},
                                {"Lemon_juice", 14.4}, {"Hot_pepper_sauce", 4.7}, {"Chicken_thigh", 340.2},
                                {"Greek_yogurt", 42.52}, {"Unsalted_butter", 4.71}},

                                {{"Onion", 125}, {"Garlic_cloves", 8.4}, {"Canola_oil", 13.4}, {"Rice", 500},
                                {"Curry_powder", 2.42}, {"Salt", 6}, {"Pepper", 0.57}, {"Garam_masala", 3.5}},

                                "db/recipes/chikenTikkaMasala/instructions.txt",

                                "High protein"};
    Recipes pizza =  {"Pizza",

                     {{"Yeast", 3.1}, {"Tomato_puree", 95.1}, {"Mozzarella", 125},
                     {"Parmesan", 10}, {"Cherry_tomatoes", 85},},

                     {{"Bread_flour", 300}, {"Salt", 5.69}, {"Olive_oil", 13.69}, 
                     {"Dried_basil", 2}, {"Garlic", 4}},

                     "db/recipes/pizza/instructions.txt",
                     
                     "Uncategorized"};
    
    Recipes lasagna = {"Lasagna",

                      {{"Ground_beef", 400}, {"Carrots", 260}, {"Celery", 300}, 
                      {"Squash", 280},{"Tomato_puree", 55}, {"Chopped_tomatoes", 800}, 
                      {"Butter", 28.35},  {"Milk", 300}, {"Mozzarella", 250}},

                      {{"Onion", 200}, {"Garlic", 24}, {"Oregano", 2}, {"Thyme", 1}, 
                      {"Vegetable_broth", 100}, {"Olive_oil", 30}, {"Wheat_flour", 15}, 
                      {"Nutmeg", 1}, {"Lasagna_Plates", 200}},

                      "db/recipes/lasagna/instructions.txt"
                      
                      "Uncategorized"};
    
    Recipes burningLove = {"Burning Love",

                          {{"Milk", 100}, {"Butter", 25},  {"Bacon", 200}, {"Pickled_beetroots", 100}},

                          {{"Potatoes", 600}, {"Salt", 2}, {"Onion", 200}},

                          "db/recipes/burninglove/instructions.txt"
                          
                          "Uncategorized"};
    
    Recipes meatLoaf = {"Meat Loaf",

                       {{"Chopped_lambmeat", 500}, {"Milk", 100}, {"Cream", 47.5},
                       {"Eggs", 100}, {"Dried_tomatoes", 20}, {"Black_olives", 50}, {"Feta", 75},
                       {"Butter", 14}},

                       {{"Onion", 200}, {"Garlic", 12}, {"Oats", 90}, {"Thyme", 18}, {"Rosemary", 6}, 
                       {"Baby_potatoes", 800}, {"Oliveoil", 15}, {"Salt_&_pepper", 1}},

                       "db/recipes/meatloaf/instructions.txt"
                       
                       "Uncategorized"};  

    Recipes ricePudding = {"Rice_pudding",

                      {{"Milk", 2000}, {"Butter", 20}},

                      {{"Porridge_rice", 484.38}, {"Water", 100}, {"Salt", 3}, {"Cinnamon", 10},
                      {"Sugar", 16}},
                      
                       "db/recipes/ricepudding/instructions.txt"
                       
                       "Uncategorized"};


    vegetarianRecipes[0] = falafel; /*https://www.tasteofhome.com/recipes/falafel-2/*/
    vegetarianRecipes[1] = macNCheese;/*https://www.tasteofhome.com/recipes/veggie-macaroni-cheese/*/
    vegetarianRecipes[2] = zucchiniMushroomBake;/*https://www.tasteofhome.com/recipes/zucchini-mushroom-bake/*/

    lowCarbRecipes[0] = ovenRoastedSalmom;/*https://www.tasteofhome.com/recipes/oven-roasted-salmon/*/
    lowCarbRecipes[1] = butterChickenMeatballs;/*https://www.tasteofhome.com/recipes/butter-chicken-meatballs/*/
    LowCarbRecipes[2] = zucchiniMushroomBake;/*https://www.tasteofhome.com/recipes/zucchini-mushroom-bake/*/

    lowCalorieRecipes[0] = coconutCurryChicken;/*https://www.tasteofhome.com/recipes/coconut-curry-chicken/*/
    lowCalorieRecipes[1] = eggplantCasserole;/*https://www.tasteofhome.com/recipes/eggplant-casserole/*/
    lowCalorieRecipes[2] = orangeBarleyChicken;/*https://www.tasteofhome.com/recipes/orange-barley-chicken/*/

    highProteinRecipes[0] = jambalaya; /*https://www.tasteofhome.com/recipes/meaty-slow-cooked-jambalaya/*/
    highProteinRecipes[1] = chickenTikkaMasala; /*https://www.tasteofhome.com/recipes/slow-cooker-chicken-tikka-masala/*/
    highProteinRecipes[2] = coconutCurryChicken; 
    
    uncategorizedRecipes[0] = pizza;
    uncategorizedRecipes[1] = lasagna;
    uncategorizedRecipes[2] = burningLove;
    uncategorizedRecipes[3] = meatLoaf;
    uncategorizedRecipes[4] = ricePudding;

    allRecipes[0] = pizza;
    allRecipes[1] = lasagna;
    allRecipes[2] = burningLove;
    allRecipes[3] = meatLoaf;
    allRecipes[4] = ricePudding;
}
