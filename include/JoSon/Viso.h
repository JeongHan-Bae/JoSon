#pragma once

#include <string>
#include <atomic>
#include "Doc.h"

namespace JoSon::Viso {

    // Add the colorful option
    /** @brief Flag to enable colorful output. */
    extern bool colorful;

    /** @brief Color for string values. */
    extern std::string str_color;

    /** @brief Color for digit values. */
    extern std::string digit_color;

    /** @brief Color for keyword values. */
    extern std::string key_color;

    /** @brief Step for updating progress bars. */
    extern unsigned int progress_step;

    /**
     * @brief Prints formatted JSON string with customizable colors.
     *
     * This function prints a formatted version of a JSON string, allowing
     * customization of colors for strings, digits, and keywords.
     *
     * @param json_str The JSON string to be printed.
     * @param indents The number of indent spaces to use.
     */
    [[maybe_unused]] void json_print(const std::string& json_str, int indents = 2);


    /**
     * @brief Represents a progress bar to visualize the progress of a task.
     *
     * This struct encapsulates the data and functionality required to display a progress
     * bar indicating the progress of a task relative to the total amount of work to be done.
     */
    struct ProgressBar {
        std::atomic<size_t>* const progress; ///< Pointer to the current progress made on the task.
        const std::atomic<size_t>* total;    ///< Pointer to the total amount of work to be done.
        int percentage;         ///< The percentage of completion of the task.


        /**
         * @brief Constructor for ProgressBar.
         *
         * Constructs a ProgressBar object with the provided pointers to progress and total,
         * initializing the percentage to 0.
         *
         * @param p Pointer to the current progress made on the task.
         * @param t Pointer to the total amount of work to be done.
         */
        ProgressBar(std::atomic<size_t>* const p, const std::atomic<size_t>* t)
                : progress(p), total(t), percentage(0) {}

        /**
         * @brief Update the progress bar.
         *
         * Updates the progress bar based on the current progress and total values.
         * If the additional progress made since the last update is less than the step
         * defined in JoSon::Viso::progress_step, the progress bar will not be updated
         * on the screen to save time.
         */
        void update();

        ~ProgressBar() = default; ///< Destructor for ProgressBar.

    };

} // namespace JoSon::Viso

