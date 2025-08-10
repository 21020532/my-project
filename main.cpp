#include <iostream>
#include "src/include/Board.h"
#include "src/include/Dictionary.h"
#include "src/include/Player.h"
#include "src/include/TileBag.h"

int main() {
    Board board;
    Dictionary dict;
    dict.loadFromFile("dictionary.txt");
    
    TileBag tileBag;
    Player player1, player2;
    int player1Score = 0, player2Score = 0;
    int currentPlayer = 1; // 1 hoặc 2
    

    player1.drawTiles(tileBag, 7);
    player2.drawTiles(tileBag, 7);

    std::cout << "=== SCRABBLE GAME - 2 PLAYERS ===\n";
    std::cout << "Player 1: Red, Player 2: Blue\n\n";

    while (true) {

        board.display();
        
        Player& currentPlayerObj = (currentPlayer == 1) ? player1 : player2;
        int& currentScore = (currentPlayer == 1) ? player1Score : player2Score;
        
        std::cout << "\n=== PLAYER " << currentPlayer << " TURN ===\n";
        currentPlayerObj.displayTiles();
        std::cout << "Player " << currentPlayer << " Score: " << currentScore << std::endl;
        std::cout << "Player 1 Total: " << player1Score << " | Player 2 Total: " << player2Score << std::endl;
        
        std::string word;
        int x, y, dir;
        std::cout << "\nEnter the word you want to set (or 'pass' to skip, 'quit' to exit): ";
        std::cin >> word;
        
        if (word == "quit") break;
        if (word == "pass") {
            std::cout << "Player " << currentPlayer << " passed their turn.\n";
          
            currentPlayer = (currentPlayer == 1) ? 2 : 1;
            continue;
        }
        

        if (!dict.isValidWord(word)) {
            std::cout << "Invalid word in dictionary!\n";
            continue;
        }
        
        std::cout << "Enter x y coordinates (0-14): ";
        std::cin >> x >> y;
        std::cout << "Enter direction (0: horizontal, 1: vertical): ";
        std::cin >> dir;
        bool horizontal = (dir == 0);

        
        if (!currentPlayerObj.hasTilesForWordWithBoard(word, board.getGrid(), x, y, horizontal)) {
            std::cout << "You don't have enough tiles for this word!\n";
            continue;
        }
        
        if (board.placeWord(word, x, y, horizontal, dict)) {
           
            int wordScore = board.calculateTotalScore(word, x, y, horizontal);
            currentScore += wordScore;
            
           
            std::cout << "DEBUG: Word '" << word << "' score: " << wordScore << std::endl;
            
            std::cout << "\n=== SUCCESS! ===\n";
            std::cout << "Player " << currentPlayer << " placed: " << word << std::endl;
            std::cout << "Word score: " << wordScore << std::endl;
            std::cout << "Player " << currentPlayer << " total score: " << currentScore << std::endl;
            

            
            std::vector<std::string> perpWords = board.getPerpendicularWords(word, x, y, horizontal);
            if (!perpWords.empty()) {
                std::cout << "Perpendicular words formed: ";
                for (const std::string& perpWord : perpWords) {
                    std::cout << perpWord << " ";
                }
                std::cout << std::endl;
            }
            
          
            currentPlayerObj.useTilesForWordWithBoard(word, board.getGrid(), x, y, horizontal);
          
            currentPlayerObj.drawTiles(tileBag, 7 - currentPlayerObj.getTileCount());
            
          
            std::cout << "\n=== SCORE UPDATE ===\n";
            std::cout << "Player 1: " << player1Score << " | Player 2: " << player2Score << std::endl;
         
            currentPlayer = (currentPlayer == 1) ? 2 : 1;
            
        } else {
            std::cout << "Cannot place word at this location!\n";
        }
    }
    
    // Hiển thị kết quả cuối cùng
    std::cout << "\n=== GAME OVER ===\n";
    std::cout << "Final Scores:\n";
    std::cout << "Player 1: " << player1Score << std::endl;
    std::cout << "Player 2: " << player2Score << std::endl;
    
    if (player1Score > player2Score) {
        std::cout << "Player 1 WINS!\n";
    } else if (player2Score > player1Score) {
        std::cout << "Player 2 WINS!\n";
    } else {
        std::cout << "It's a TIE!\n";
    }
    
    return 0;
}
