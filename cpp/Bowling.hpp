// From 2019 in my apprenticeship (German "Ausbildung").
// Supervisor taught me about fast reporting, iterating and being clean in code.

#pragma once

#include <vector>
#include <stdexcept>

namespace bowling
{
  class Frame
  {
    public:
      Frame() : m_score(0) {}

      void addRoll(unsigned int pins)
      {
        m_rolls.push_back(pins);
        m_score += pins;
      }

      void addBonus(unsigned int pins)
      {
        m_score += pins;
      }

      unsigned int score() const { return m_score; }

      size_t rollCount() const { return m_rolls.size(); }

      unsigned int getRoll(size_t index) const
      {
        return index < m_rolls.size() ? m_rolls[index] : 0;
      }

      unsigned int pinsDown() const
      {
        unsigned int total = 0;
        for (auto pins : m_rolls)
        {
          total += pins;
        }
        return total;
      }

      bool isStrike() const
      {
        return !m_rolls.empty() && m_rolls[0] == 10;
      }

      bool isSpare() const
      {
        return m_rolls.size() >= 2 && !isStrike() && (m_rolls[0] + m_rolls[1] == 10);
      }

      const std::vector<unsigned int>& rolls() const { return m_rolls; }

    private:
      std::vector<unsigned int> m_rolls;
      unsigned int m_score;
  };

  class Game
  {
    public:
      static constexpr unsigned int TOTAL_FRAMES   = 10;
      static constexpr unsigned int PINS_PER_FRAME = 10;
      static constexpr unsigned int PERFECT_SCORE  = 300;

      Game()
      {
        m_frames.reserve(TOTAL_FRAMES);

        for (unsigned int i = 0; i < TOTAL_FRAMES; ++i)
        {
          m_frames.emplace_back();
        }
      }

      void recordRoll(unsigned int pins)
      {
        if (checkIfGameIsOver())
        {
          throw std::runtime_error("Game is over! No more rolls allowed!");
        }

        if (pins > PINS_PER_FRAME)
        {
          throw std::runtime_error("Invalid roll: Cannot knock down more than 10 pins!");
        }

        Frame& frame = m_frames[m_currentFrame];

        // Validate pins don't exceed remaining pins in frame:
        if (m_currentFrame < 9)
        {
          // Frames 1-9: can't knock down more pins than standing:
          if (frame.rollCount() == 1 && frame.getRoll(0) + pins > 10)
          {
            throw std::runtime_error("Invalid roll: Exceeds remaining pins!");
          }
        }
        else
        {
          // 10th frame: reset pins after strike or spare:
          if (frame.rollCount() == 1 && !frame.isStrike() && frame.getRoll(0) + pins > 10)
          {
            throw std::runtime_error("Invalid roll: Exceeds remaining pins!");
          }
          if (frame.rollCount() == 2 && frame.isSpare() && pins > 10)
          {
            throw std::runtime_error("Invalid roll: Exceeds remaining pins!");
          }
          if (frame.rollCount() == 2 && frame.isStrike())
          {
            // After strike, second roll can be 0-10.
            // If second roll is also strike, third can be 0-10.
            // If second roll is not strike, third is limited.
            if (frame.getRoll(1) < 10 && frame.getRoll(1) + pins > 10)
            {
              throw std::runtime_error("Invalid roll: Exceeds remaining pins!");
            }
          }
        }

        m_allRolls.push_back(pins);

        // Apply bonus from previous spare:
        if (m_currentFrame >= 1)
        {
          Frame& prevFrame = m_frames[m_currentFrame - 1];
          if (frame.rollCount() == 0 && prevFrame.isSpare())
          {
            prevFrame.addBonus(pins);
          }
        }

        // Apply bonus from previous strike(s):
        if (m_currentFrame >= 1)
        {
          Frame& prevFrame = m_frames[m_currentFrame - 1];
          if (prevFrame.isStrike() && frame.rollCount() <= 1)
          {
            prevFrame.addBonus(pins);
          }
        }
        if (m_currentFrame >= 2)
        {
          Frame& prevPrevFrame = m_frames[m_currentFrame - 2];
          if (prevPrevFrame.isStrike() && m_frames[m_currentFrame - 1].isStrike() && frame.rollCount() == 0)
          {
            prevPrevFrame.addBonus(pins);
          }
        }

        frame.addRoll(pins);

        // Move to next frame (except in 10th frame):
        if (m_currentFrame < 9)
        {
          if (frame.isStrike() || frame.rollCount() == 2)
          {
            m_currentFrame++;
          }
        }
      }

      // Get all frames:
      const std::vector<Frame>& frames() const
      {
        return m_frames;
      }

      unsigned int calculateTotalScore() const
      {
        unsigned int total = 0;
        for (const auto& frame : m_frames)
        {
          total += frame.score();
        }
        return total;
      }

      bool checkIfGameIsOver() const
      {
        if (m_currentFrame < 9)
        {
          return false;
        }

        const Frame& tenthFrame = m_frames[9];

        // 3 rolls in 10th frame means done:
        if (tenthFrame.rollCount() == 3)
        {
          return true;
        }

        // 2 rolls without strike or spare means done:
        if (tenthFrame.rollCount() == 2 && tenthFrame.pinsDown() < 10)
        {
          return true;
        }

        return false;
      }

      unsigned int getCurrentFrameNumber() const
      {
        return m_currentFrame;
      }

      const std::vector<unsigned int>& getAllRolls() const
      {
        return m_allRolls;
      }

      bool checkIfItIsPerfectSoFar() const
      {
        for (auto pins : m_allRolls)
        {
          if (pins != 10) return false;
        }
        return true;
      }

    private:
      std::vector<Frame> m_frames;
      std::vector<unsigned int> m_allRolls;
      unsigned int m_currentFrame = 0;
    };

} // namespace bowling
