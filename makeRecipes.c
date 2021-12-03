#include <stdio.h>
#include <stdlib.h>
#include "app.h"

void makeRecipes (ingredients *fridgeContents, Recipes *vegetarianRecipes, Recipes *lowCarbRecipes, Recipes *lowCalorieRecipes, Recipes *highProteinRecipes, Recipes *allRecipes){
/*Declaration of different recipes*/
    Recipes macNCheese =  {"MacNCheese",

                     {{"Broccoli", 210}, {"Cauliflower", 130}, {"Carrots", 165}, {"Celery", 80}, {"Butter", 12}, {"Milk", 240}, {"Cheddar", 250}, {"Dijon_mustard", 15}}, 

                     {{"Onion", 150}, {"Wheat_flour", 32}, {"Chicken_broth", 100}, {"Salt", 2}, {"Pepper", 1}, {"Paprika", 2}},

                     "db/recipes/macNCheese/instructions.txt",
                     
                     "Vegetarian",
                     
                     "1 cup: 200 calories, 11g fat (6g saturated fat), 33mg cholesterol, 391mg sodium, 15g carbohydrate (3g sugars, 2g fiber), 10g protein."};


    Recipes falafel =  {"Falafel",

                     {{}},

                     {{"Dried_Chickpeas", 170}, {"Baking_soda", 3}, {"Cilantro_leaves", 17}, {"Mint_leaves", 15}, 
                     {"Garlic_cloves", 20}, {"Ground_coriander", 6}, {"Chili_powder", 6}, {"Salt", 6}, {"Pepper", 3}, {"Sesame_seeds", 6}, {"Baking_powder", 6}},

                     "db/recipes/falafel/instructions.txt",
                     
                     "Vegetarian",
                     
                     "4 pieces: 224 calories, 13g fat (1g saturated fat), 0 cholesterol, 760mg sodium, 32g carbohydrate (1g sugars, 16g fiber), 9g protein."};

    Recipes zucchiniMushroomBake =  {"Zucchini_Mushroom_Bake",

                     {{"Squash", 450}, {"Fresh_Mushroom", 150}, {"Cheddar", 120}},

                     {{"Onion", 20}, {"Dried_basil", 3}, {"Salt", 2}},

                     "db/recipes/zucchiniMushroomBake/instructions.txt",
                     
                     "Vegetarian_and_Low_carb",
                     
                     "2/3 cup: 83 calories, 5g fat (3g saturated fat), 14mg cholesterol, 249mg sodium, 5g carbohydrate (3g sugars, 1g fiber), 5g protein. Diabetic Exchanges: 1 medium-fat meat, 1 vegetable."};

    Recipes ovenRoastedSalmom =  {"Oven_Roasted_Salmom",

                     {{"Salmon_fillet", 675}},

                     {{"Olive_oil", 15}, {"Salt", 3}, {"Pepper", 2}},

                     "db/recipes/ovenRoastedSalmom/instructions.txt",
                     
                     "Low_carb",
                     
                     "1 fillet: 295 calories, 19g fat (4g saturated fat), 85mg cholesterol, 380mg sodium, 0 carbohydrate (0 sugars, 0 fiber), 29g protein. Diabetic Exchanges: 4 lean meat, 1/2 fat."};


    Recipes butterChickenMeatballs =  {"Butter_Chicken_Meatballs",

                     {{"Ground_chicken", 675}, {"Egg", 60}, {"Butter_chicken_sauce", 400}},

                     {{"Bread_crumbs", 60}, {"Garam_masala", 6}, {"Tandoori_masala" 3}, {"Salt", 3}, {"Cayenne_pepper", 2}, {"Cilantro", 5}},

                     "db/recipes/butterChickenMeatballs/instructions.txt",
                     
                     "Low_carb",
                     
                     "1 meatball: 40 calories, 2g fat (1g saturated fat), 18mg cholesterol, 87mg sodium, 1g carbohydrate (1g sugars, 0 fiber), 3g protein."};


    Recipes coconutCurryChicken =  {"Coconut_Curry_Chicken",

                     {{"Potatos", 300}, {"Chicken_breast", 200}, {"Cooked_rice", 500}},

                     {{"Onion", 115}, {"Canola_oil", 12}, {"Coconut_milk", 240}, {"Curry_powder", 24}, 
                     {"Garlic_cloves", 5}, {"Chicken_bouillon_granules", 6}, {"Salt", 2}, {"Pepper", 2}},

                     "db/recipes/coconutCurryChicken/instructions.txt",
                     
                     "Low_calorie",
                     
                     "1 serving: 371 calories, 10g fat (4g saturated fat), 63mg cholesterol, 265mg sodium, 42g carbohydrate (3g sugars, 3g fiber), 27g protein. Diabetic Exchanges: 3 starch, 3 lean meat, 1-1/2 fat."};

    Recipes eggplantCasserole =  {"Eggplant_Casserole",

                     {{"Eggplant", 550}, {"Ground_beef", 675}, {"Green_pepper", 120}, {"Tomato", 180}, {"Milk", 120}, {"Egg", 65}, {"Butter", 30}},

                     {{"Water", 950}, {"Onion", 150}, {"Bread_crumbs", 50}},

                     "db/recipes/eggplantCasserole/instructions.txt",
                     
                     "Low_calorie",
                     
                     "1-1/3 cups: 342 calories, 19g fat (8g saturated fat), 113mg cholesterol, 186mg sodium, 18g carbohydrate (7g sugars, 4g fiber), 25g protein."};

    Recipes orangeBarleyChicken =  {"Orange_Barley_Chicken",

                     {{"Chicken", 1800}, {"Butter", 14}, {"Orange_juice", 300}, {"Parsnip", 300}, {"Pearl_barley", 260}, {"Fresh_mushroom", 225}},

                     {{"Wheat_flour", 150}, {"Celery_salt", 6}, {"Pepper", 3}, {"Olive_oil", 12}, {"Onion", 150}, {"Chicken_broth", 300}, {"Bay_leave", 0,5}},

                     "db/recipes/orangeBarleyChicken/instructions.txt",
                     
                     "Low_calorie",
                     
                     "1 serving: 384 calories, 8g fat (2g saturated fat), 59mg cholesterol, 349mg sodium, 54g carbohydrate (13g sugars, 8g fiber), 26g protein."};
    
    Recipes jambalaya = {"Jambalaya",

                        {{"Diced_tomatoes", 793.8}, {"Reduced-sodium_chicken_broth", 240}, {"Green_pepper", 164},
                        {"Celery_ribs", 110}, {"White_wine", 117.2}, {"Chicken", 907.2},
                        {"Chicken_sausage", 340.2}, {"Medium_shrimp", 907.2}},

                        {{"Onion", 170}, {"Garlic_cloves", 11.2}, {"Cajun_seasosning", 8}, {"Dried_parsley", 1.58},
                        {"Dried_basil", 0.71}, {"Dried_oregano", 1.01}, {"Salt", 4.5}, {"Cayenne_pepper", 1.8},
                        {"Brown_rice", 1520}},

                        "db/recipes/Jambalaya/instructions.txt",

                        "High protein"}

    Recipes chickenTikkaMasala = {"Chicken Tikka Masala",
    
                                {{"Tomato_puree", 425.24}, {"Tomato_paste", 28.1}, {"Grated_lemon_zest", 4.2},
                                {"Lemon_juice", 14.4}, {"Hot_pepper_sauce", 4.7}, {"Chicken", 340.2},
                                {"Greek_yogurt", 42.52}, {"Unsalted_butter", 4.71}},

                                {{"Onion", 125}, {"Garlic_cloves", 8.4}, {"Canola_oil", 13.4}, {"Rice", 500},
                                {"Curry_powder", 2.42}, {"Salt", 6}, {"Pepper", 0.57}, {"Garam_masala", 3.5}},

                                "db/recipes/chikenTikkaMasala/instructions.txt",

                                "High protein"};



    


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
    

    allRecipes[0] = falafel;
    allRecipes[1] = macNCheese;
    allRecipes[2] = zucchiniMushroomBake;
    allRecipes[3] = ovenRoastedSalmom;
    allRecipes[4] = butterChickenMeatballs;
    allRecipes[5] = jambalaya;
    allRecipes[6] = coconutCurryChicken;
    allRecipes[7] = eggplantCasserole;
    allRecipes[8] = orangeBarleyChicken;
    allRecipes[9] = chickenTikkaMasala
    

}
