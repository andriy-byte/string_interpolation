#ifndef STRING_INTERPOLATION_H
#define STRING_INTERPOLATION_H

#include <concepts>
#include <map>
#include <string>
#include <cassert>
#include <sstream>
#include <iostream>

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif


namespace istr {

#if defined(STRING_INTERPOLATION_EXTEND_TYPES)
    template<typename Type>
    concept CharTypes = std::is_same<Type, char>::value || std::is_same<Type, char8_t>::value ||
                        std::is_same<Type, char16_t>::value || std::is_same<Type, char32_t>::value ||
                        std::is_same<Type,wchar_t>::value;
#elif
    template<typename Type>
    concept CharTypes = std::is_same<Type, char>::value;
#endif

    enum class Modes {
        WITH_REPLACEMENT_VALIDATION,
        WITHOUT_REPLACEMENT_VALIDATION
    };

    template<CharTypes T>
    class BasicStringInterpolation {
    public:
        BasicStringInterpolation() noexcept;

        BasicStringInterpolation(const std::basic_string<T>& pattern,
                                          const std::map<std::basic_string<T>,std::basic_string<T>>& patternArguments,
                                          const std::basic_string<T>& openDelimiter,
                                          const std::basic_string<T>& closeDelimiter) noexcept;

        BasicStringInterpolation(
                const std::basic_string<T>& pattern,
                const std::map<std::basic_string<T>, std::basic_string<T>>& patternArguments) noexcept;

        BasicStringInterpolation(
                const std::basic_string<T>& pattern) noexcept;

        BasicStringInterpolation(
                const std::basic_string<T>& pattern,
                const std::basic_string<T>& openDelimiter,
                const std::basic_string<T>& closeDelimiter) noexcept;

        BasicStringInterpolation(Modes mode) noexcept;

        BasicStringInterpolation(const std::basic_string<T>& pattern,
                                          const std::map<std::basic_string<T>,std::basic_string<T>>& patternArguments,
                                          const std::basic_string<T>& openDelimiter,
                                          const std::basic_string<T>& closeDelimiter, Modes mode) noexcept;

        BasicStringInterpolation(
                const std::basic_string<T>& pattern,
                const std::map<std::basic_string<T>, std::basic_string<T>>& patternArguments,
                Modes mode) noexcept;

        BasicStringInterpolation(
                const std::basic_string<T>& pattern,
                const std::basic_string<T>& openDelimiter,
                const std::basic_string<T>& closeDelimiter, Modes mode) noexcept;

        BasicStringInterpolation(const std::basic_string<T>& pattern, Modes mode) noexcept;

        static const std::basic_string<T>& getDefaultOpenDelimiter() noexcept;

        static const std::basic_string<T>& getDefaultCloseDelimiter() noexcept;

        const std::basic_string<T>& getOpenDelimiter() const noexcept;

        void setOpenDelimiter(const std::basic_string<T>& openDelimiter) const noexcept;

        const std::basic_string<T>& getCloseDelimiter() const noexcept;

        void setCloseDelimiter(const std::basic_string<T>& closeDelimiter) const noexcept;

        const std::basic_string<T>& getPattern() const noexcept;

        void setPattern(const std::basic_string<T>& pattern) noexcept;

        const std::map<std::basic_string<T>, std::basic_string<T>>&
        getPatternArguments() const noexcept;

        void putPatternArgument(const std::basic_string<T>& replaceable,
                                const std::basic_string<T>& replacement) const noexcept;

        void putPatternArguments(const std::map<std::basic_string<T>, std::basic_string<T>>& patternArguments) const noexcept;

        const std::basic_string<T> getSubstituted() const noexcept;

        BasicStringInterpolation<T>& operator+=(const BasicStringInterpolation<T>& templateBasicString) noexcept;

        void setOpenAndCloseDelimiters(const std::basic_string<T>& openDelimiter,
                                       const std::basic_string<T>& closeDelimiter) const noexcept;

        Modes getMode() const noexcept;

        void setMode(const Modes mode) const noexcept;

        void prepareSubstitution() const noexcept;

    private:
        void tryPrepareSubstitution() const noexcept(false);

        std::basic_string<T> validateReplaceable(const std::basic_string<T>& replaceable) const noexcept;

        mutable bool alreadySubstituted = false;
        std::basic_string<T> pattern;
        mutable std::basic_string<T> substitution;
        mutable std::map<std::basic_string<T>, std::basic_string<T>> patternArguments;
        static std::basic_string<T> defaultOpenDelimiter, defaultCloseDelimiter;
        mutable std::basic_string<T> openDelimiter, closeDelimiter;
        mutable Modes mode;
    };

    template<CharTypes T>
    std::basic_string<T> BasicStringInterpolation<T>::defaultOpenDelimiter(
            static_cast<std::basic_string<T>>("{{"));
    template<CharTypes T>
    std::basic_string<T> BasicStringInterpolation<T>::defaultCloseDelimiter(
            static_cast<std::basic_string<T>>("}}"));

    template<CharTypes T>
    void BasicStringInterpolation<T>::prepareSubstitution() const noexcept{
        if (this->mode == Modes::WITH_REPLACEMENT_VALIDATION) {
            tryPrepareSubstitution();
        }

        this->substitution = this->pattern;

        for (const auto& [replaceable, replacement]: this->getPatternArguments()) {
            int start_position = 0;
            while ((start_position = this->substitution.find(replaceable, start_position)) !=
                    std::basic_string< T, std::char_traits<T>, std::allocator<T>> ::npos) {
                this->substitution.replace(start_position, replaceable.length(), replacement);
                start_position += replacement.length();
            }
        }
        this->alreadySubstituted = true;
    }

    template<CharTypes T>
    std::basic_string<T> BasicStringInterpolation<T>::validateReplaceable(const std::basic_string<T>& replaceable) const noexcept{
        if (replaceable.starts_with(this->getOpenDelimiter()) && replaceable.ends_with(this->getCloseDelimiter())) {
            return replaceable;
        }

        return this->getOpenDelimiter() + replaceable + this->getCloseDelimiter();
    }

    template<CharTypes T>
    const std::basic_string<T>& BasicStringInterpolation<T>::getDefaultOpenDelimiter() noexcept {
        return defaultOpenDelimiter;
    }

    template<CharTypes T>
    const std::basic_string<T>& BasicStringInterpolation<T>::getDefaultCloseDelimiter() noexcept {
        return defaultCloseDelimiter;
    }

    template<CharTypes T>
    const std::basic_string<T>& BasicStringInterpolation<T>::getOpenDelimiter() const noexcept{
        return this->openDelimiter;
    }

    template<CharTypes T>
    void BasicStringInterpolation<T>::setOpenDelimiter(const std::basic_string<T>& openDelimiter) const noexcept{
        this->openDelimiter = openDelimiter;
    }

    template<CharTypes T>
    const std::basic_string<T>& BasicStringInterpolation<T>::getCloseDelimiter() const noexcept{
        return this->closeDelimiter;
    }

    template<CharTypes T>
    void BasicStringInterpolation<T>::setCloseDelimiter(
            const std::basic_string<T>& closeDelimiter) const noexcept{
        this->closeDelimiter = closeDelimiter;
    }

    template<CharTypes T>
    const std::basic_string<T>& BasicStringInterpolation<T>::getPattern() const noexcept{
        return this->pattern;
    }

    template<CharTypes T>
    void BasicStringInterpolation<T>::setPattern(
            const std::basic_string<T>& pattern) noexcept{

        this->alreadySubstituted = false;
        this->pattern = pattern;
    }

    template<CharTypes T>
    const std::map<std::basic_string<T>, std::basic_string<T>>&
    BasicStringInterpolation<T>::getPatternArguments() const noexcept{
        return this->patternArguments;
    }

    template<CharTypes T>
    void BasicStringInterpolation<T>::putPatternArguments(
            const std::map<std::basic_string<T>, std::basic_string<T>>& patternArguments) const noexcept{
        for (const auto& [replaceable, replacement]: patternArguments){
            this->putPatternArgument(replaceable, replacement);
        }
    }

    template<CharTypes T>
    BasicStringInterpolation<T>::BasicStringInterpolation(
            const std::basic_string<T>& pattern,
            const std::map<std::basic_string<T>, std::basic_string<T>>& patternArguments) noexcept{
        this->setPattern(pattern);
        this->setOpenAndCloseDelimiters(defaultOpenDelimiter, defaultCloseDelimiter);
        this->putPatternArguments(patternArguments);
        this->setMode(Modes::WITH_REPLACEMENT_VALIDATION);
    }

    template<CharTypes T>
    BasicStringInterpolation<T>::BasicStringInterpolation(const std::basic_string<T>& pattern,
                                                          const std::map<std::basic_string<T>, std::basic_string<T>>& patternArguments,
                                                          const std::basic_string<T>& openDelimiter,
                                                          const std::basic_string<T>& closeDelimiter) noexcept :
                                                          openDelimiter(openDelimiter), closeDelimiter(closeDelimiter){
        this->setPattern(pattern);
        this->putPatternArguments(patternArguments);
        this->setMode(Modes::WITH_REPLACEMENT_VALIDATION);
    }

    template<CharTypes T>
    const std::basic_string<T> BasicStringInterpolation<T>::getSubstituted() const noexcept{
        if (!this->alreadySubstituted) {
            this->prepareSubstitution();
        }
        return this->substitution;
    }

    template<CharTypes T>
    void BasicStringInterpolation<T>::putPatternArgument(const std::basic_string<T>& replaceable,
                                                         const std::basic_string<T>& replacement) const noexcept{

        if (this->pattern.contains(this->validateReplaceable(replaceable))) {
            this->patternArguments.emplace(this->validateReplaceable(replaceable), replacement);
        }
    }

    template<CharTypes T>
    BasicStringInterpolation<T>::BasicStringInterpolation(const std::basic_string<T>& pattern) noexcept{
        this->setPattern(pattern);
        this->setOpenAndCloseDelimiters(defaultOpenDelimiter, defaultCloseDelimiter);
        this->setMode(Modes::WITH_REPLACEMENT_VALIDATION);
    }
    template<CharTypes T>
    [[maybe_unused]] void BasicStringInterpolation<T>::tryPrepareSubstitution() const noexcept(false) {

        [[maybe_unused]] std::stringstream stringStream;
        stringStream << "\n";
        stringStream << "exception in file : " << __FILE__ << "\n"
        << "inside function : " << __PRETTY_FUNCTION__ << "\n"
        << "at line : " << __LINE__ << "\n";


        try {
            if (this->getPattern().empty()) {
                stringStream << "exception message : "
                << "it is impossible to build a string template, there is no template"
                << "\n";
                throw std::runtime_error(stringStream.str());
            }
            if (this->getPatternArguments().empty()) {
                stringStream << "exception message : "
                << "has no arguments to replace in pattern , cannot do substitution string"
                << "\n";
                throw std::runtime_error(stringStream.str());
            }
            for (const auto& [key, value]: this->getPatternArguments()) {
                if (!this->getPattern().contains(key)) {
                    stringStream << "exception message : "
                    << "the string template's pattern \" " +
                    static_cast<std::basic_string<T>>(this->getPattern()) +
                    " \" does not contain a argument " +
                    static_cast<std::basic_string<T>>(key) + "."
                    << "\n";
                    throw std::runtime_error(stringStream.str());
                }
            }
        } catch (std::exception const& substitutionPrepareException) {
            std::cerr << substitutionPrepareException.what();
        } catch (...) {
            std::cerr << stringStream.str();
        }
    }

    template<CharTypes T>
    BasicStringInterpolation<T>&
    BasicStringInterpolation<T>::operator+=(const BasicStringInterpolation<T>& templateBasicString) noexcept{
        this->pattern += templateBasicString.getPattern();
        this->putPatternArguments(templateBasicString.getPatternArguments());
        return *this;
    }

    template<CharTypes T>
    void BasicStringInterpolation<T>::setOpenAndCloseDelimiters(const std::basic_string<T>& openDelimiter,
                                                                const std::basic_string<T>& closeDelimiter) const noexcept{
        this->setOpenDelimiter(openDelimiter);
        this->setCloseDelimiter(closeDelimiter);
    }

    template<CharTypes T>
    BasicStringInterpolation<T>::BasicStringInterpolation() noexcept{
        this->setOpenAndCloseDelimiters(defaultOpenDelimiter, defaultCloseDelimiter);
        this->setMode(Modes::WITH_REPLACEMENT_VALIDATION);
    }

    template<CharTypes T>
    BasicStringInterpolation<T>::BasicStringInterpolation(const std::basic_string<T>& pattern,
                                                          const std::basic_string<T>& openDelimiter,
                                                          const std::basic_string<T>& closeDelimiter) noexcept :
                                                          openDelimiter(openDelimiter), closeDelimiter(closeDelimiter) {
        this->setPattern(pattern);
        this->setMode(Modes::WITH_REPLACEMENT_VALIDATION);
    }

    template<CharTypes T>
    Modes BasicStringInterpolation<T>::getMode() const noexcept{
        return this->modes;
    }

    template<CharTypes T>
    void BasicStringInterpolation<T>::setMode(const Modes mode) const noexcept{
        this->mode = mode;
    }

    template<CharTypes T>
    BasicStringInterpolation<T>::BasicStringInterpolation(Modes mode) noexcept{
        this->setOpenAndCloseDelimiters(defaultOpenDelimiter, defaultCloseDelimiter);
        this->setMode(mode);
    }

    template<CharTypes T>
    BasicStringInterpolation<T>::BasicStringInterpolation(const std::basic_string<T>& pattern,
                                                          const std::map<std::basic_string<T>, std::basic_string<T>>& patternArguments,
                                                          const std::basic_string<T>& openDelimiter,
                                                          const std::basic_string<T>& closeDelimiter, Modes mode) noexcept :
                                                          openDelimiter(openDelimiter), closeDelimiter(closeDelimiter) {
        this->setPattern(pattern);
        this->putPatternArguments(patternArguments);
        this->setMode(mode);
    }

    template<CharTypes T>
    BasicStringInterpolation<T>::BasicStringInterpolation(const std::basic_string<T>& pattern,
                                                          const std::map<std::basic_string<T>, std::basic_string<T>>& patternArguments,
                                                          Modes mode) noexcept{
        this->setPattern(pattern);
        this->setOpenAndCloseDelimiters(defaultOpenDelimiter, defaultCloseDelimiter);
        this->putPatternArguments(patternArguments);
        this->setMode(mode);
    }

    template<CharTypes T>
    BasicStringInterpolation<T>::BasicStringInterpolation(const std::basic_string<T>& pattern, Modes mode) noexcept{
        this->setPattern(pattern);
        this->setOpenAndCloseDelimiters(defaultOpenDelimiter, defaultCloseDelimiter);
        this->setMode(mode);
    }

    template<CharTypes T>
    BasicStringInterpolation<T>::BasicStringInterpolation(const std::basic_string<T>& pattern,
                                                          const std::basic_string<T>& openDelimiter,
                                                          const std::basic_string<T>& closeDelimiter, Modes mode) noexcept :
                                                          openDelimiter(openDelimiter), closeDelimiter(closeDelimiter) {
        this->setPattern(pattern);
        this->setMode(mode);
    }

    using StringInterpolation [[maybe_unused]] = BasicStringInterpolation<char>;
#if defined(STRING_INTERPOLATION_EXTEND_TYPES)
    using StringInterpolationU8 [[maybe_unused]] = BasicStringInterpolation<char8_t>;
    using StringInterpolationU16 [[maybe_unused]] = BasicStringInterpolation<char16_t>;
    using StringInterpolationU32 [[maybe_unused]] = BasicStringInterpolation<char32_t>;
    using StringInterpolationW [[maybe_unused]] = BasicStringInterpolation<wchar_t>;
#endif

} // namespace istr

#endif // STRING_INTERPOLATION_H