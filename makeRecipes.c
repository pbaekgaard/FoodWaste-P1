#include <stdio.h>
#include <stdlib.h>
#include "app.h"

void makeRecipes (ingredients *fridgeContents, Recipes *vegetarianRecipes, Recipes *lowCarbRecipes, Recipes *lowCalorieRecipes, Recipes *highProteinRecipes, Recipes *allRecipes){
/*Declaration of different recipes*/
    Recipes macNCheese =  {"MacNCheese",

                        {{"Broccoli", 210}, {"Cauliflower", 130}, {"Carrots", 165}, {"Celery", 80}, {"Butter", 12}, {"Milk", 240}, {"Cheddar", 250}, {"Dijon mustard", 15}, {"Chicken broth", 100}}, 

                        {{"Onion", 150}, {"Wheat flour", 32}, {"Salt", 2}, {"Pepper", 1}, {"Paprika", 2}},

                        "db/recipes/macNCheese/instructions.txt",
                     
                        "Vegetarian"};


    Recipes falafel =  {"Falafel",

                        {{"Eggs", 60}},

                        {{"Dried Chickpeas", 170}, {"Baking soda", 3}, {"Cilantro leaves", 17}, {"Mint leaves", 15}, 
                        {"Garlic cloves", 20}, {"Ground coriander", 6}, {"Chili powder", 6}, {"Salt", 6}, {"Pepper", 3}, {"Sesame seeds", 6}, {"Baking powder", 6}},

                        "db/recipes/falafel/instructions.txt",
                     
                        "Vegetarian"};

    Recipes zucchiniMushroomBake =  {"Zucchini Mushroom Bake",

                        {{"Squash", 450}, {"Fresh mushroom", 150}, {"Cheddar", 120}},

                        {{"Onion", 20}, {"Dried basil", 3}, {"Salt", 2}},

                        "db/recipes/zucchiniMushroomBake/instructions.txt",
                     
                        "Vegetarian and Low carb"};

    Recipes ovenRoastedSalmom =  {"Oven Roasted Salmom",

                        {{"Salmon fillet", 675}},

                        {{"Olive oil", 15}, {"Salt", 3}, {"Pepper", 2}},

                        "db/recipes/ovenRoastedSalmom/instructions.txt",
                     
                        "Low carb"};


    Recipes butterChickenMeatballs =  {"Butter Chicken Meatballs",

                        {{"Chicken", 675}, {"Eggs", 60}, {"Butter chicken sauce", 400}},

                        {{"Bread crumbs", 60}, {"Garam masala", 6}, {"Tandoori masala", 3}, {"Salt", 3}, {"Cayenne pepper", 2}, {"Cilantro", 5}},

                        "db/recipes/butterChickenMeatballs/instructions.txt",
                     
                        "Low carb"};


    Recipes coconutCurryChicken =  {"Coconut Curry Chicken",

                        {{"Chicken", 200}, {"Cooked rice", 500}},

                        {{"Onion", 115}, {"Canola oil", 12}, {"Coconut milk", 240}, {"Curry powder", 24}, 
                        {"Garlic cloves", 5}, {"Chicken bouillon granules", 6}, {"Salt", 2}, {"Pepper", 2}, {"Potatos", 300}},

                        "db/recipes/coconutCurryChicken/instructions.txt",
                     
                        "Low calorie"};

    Recipes eggplantCasserole =  {"Eggplant Casserole",

                        {{"Eggplant", 550}, {"Ground beef", 675}, {"Green pepper", 120}, {"Tomato", 180}, {"Milk", 120}, {"Eggs", 65}, {"Butter", 30}},

                        {{"Water", 950}, {"Onion", 150}, {"Bread crumbs", 50}},

                        "db/recipes/eggplantCasserole/instructions.txt",
                     
                        "Low calorie"};

    Recipes orangeBarleyChicken =  {"Orange Barley Chicken",

                        {{"Chicken", 1800}, {"Butter", 14}, {"Orange juice", 300}, {"Parsnip", 300}, {"Pearl barley", 260}, {"Fresh mushroom", 225}, {"Chicken broth", 300}},

                        {{"Wheat flour", 150}, {"Celery salt", 6}, {"Pepper", 3}, {"Olive oil", 12}, {"Onion", 150}, {"Bay leave", 0.5}},

                        "db/recipes/orangeBarleyChicken/instructions.txt",

                        "Low calorie"};

    Recipes jambalaya = {"Jambalaya",

                        {{"Tomato", 793.8}, {"Chicken broth", 240}, {"Green pepper", 164},
                        {"Celery ribs", 110}, {"White wine", 117.2}, {"Chicken", 907.2},
                        {"Chicken sausage", 340.2}, {"Medium shrimp", 907.2}},

                        {{"Onion", 170}, {"Garlic cloves", 11.2}, {"Cajun seasosning", 8}, {"Dried parsley", 1.58},
                        {"Dried basil", 0.71}, {"Dried oregano", 1.01}, {"Salt", 4.5}, {"Cayenne pepper", 1.8},
                        {"Brown rice", 1520}},

                        "db/recipes/Jambalaya/instructions.txt",

                        "High protein"};

    Recipes chickenTikkaMasala = {"Chicken Tikka Masala",
    
                        {{"Tomato puree", 425.24}, {"Tomato paste", 28.1}, {"Grated lemon zest", 4.2},
                        {"Lemon juice", 14.4}, {"Hot pepper sauce", 4.7}, {"Chicken", 340.2},
                        {"Greek yogurt", 42.52}, {"Butter", 4.71}},

                        {{"Onion", 125}, {"Garlic cloves", 8.4}, {"Canola oil", 13.4}, {"Rice", 500},
                        {"Curry powder", 2.42}, {"Salt", 6}, {"Pepper", 0.57}, {"Garam masala", 3.5}},

                        "db/recipes/chickenTikkaMasala/instructions.txt",

                        "High protein"};

    vegetarianRecipes[0] = falafel;                 /*https://www.tasteofhome.com/recipes/falafel-2/*/
    vegetarianRecipes[1] = macNCheese;              /*https://www.tasteofhome.com/recipes/veggie-macaroni-cheese/*/
    vegetarianRecipes[2] = zucchiniMushroomBake;    /*https://www.tasteofhome.com/recipes/zucchini-mushroom-bake/*/

    lowCarbRecipes[0] = ovenRoastedSalmom;          /*https://www.tasteofhome.com/recipes/oven-roasted-salmon/*/
    lowCarbRecipes[1] = butterChickenMeatballs;     /*https://www.tasteofhome.com/recipes/butter-chicken-meatballs/*/
    lowCarbRecipes[2] = zucchiniMushroomBake;       /*https://www.tasteofhome.com/recipes/zucchini-mushroom-bake/*/

    lowCalorieRecipes[0] = coconutCurryChicken;     /*https://www.tasteofhome.com/recipes/coconut-curry-chicken/*/
    lowCalorieRecipes[1] = eggplantCasserole;       /*https://www.tasteofhome.com/recipes/eggplant-casserole/*/
    lowCalorieRecipes[2] = orangeBarleyChicken;     /*https://www.tasteofhome.com/recipes/orange-barley-chicken/*/

    highProteinRecipes[0] = jambalaya;              /*https://www.tasteofhome.com/recipes/meaty-slow-cooked-jambalaya/*/
    highProteinRecipes[1] = chickenTikkaMasala;     /*https://www.tasteofhome.com/recipes/slow-cooker-chicken-tikka-masala/*/
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
    allRecipes[9] = chickenTikkaMasala;
}
