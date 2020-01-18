/*
 html_outline_representation.h     MindForger thinking notebook

 Copyright (C) 2016-2020 Martin Dvorak <martin.dvorak@mindforger.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef M8R_HTML_OUTLINE_REPRESENTATION_H_
#define M8R_HTML_OUTLINE_REPRESENTATION_H_

#include <string>
#include <vector>

#include "../../model/note.h"
#include "../markdown/markdown_outline_representation.h"
#include "../markdown/markdown_transcoder.h"
#if defined  MF_MD_2_HTML_CMARK
#include "../markdown/cmark_gfm_markdown_transcoder.h"
#else
#include "../markdown/discount_markdown_transcoder.h"
#endif

namespace m8r {

/**
 * @brief The HTML colors representation.
 *
 * Interface that provides colors for themed HTML rendering.
 */
class HtmlColorsRepresentation
{
public:
    virtual std::string& getHtmlTextColor() = 0;
    virtual std::string& getHtmlBackgroundColor() = 0;
};

class HtmlExportColorsRepresentation : public HtmlColorsRepresentation
{
public:
    explicit HtmlExportColorsRepresentation() {}
    HtmlExportColorsRepresentation(const HtmlExportColorsRepresentation&) = delete;
    HtmlExportColorsRepresentation(const HtmlExportColorsRepresentation&&) = delete;
    HtmlExportColorsRepresentation &operator=(const HtmlExportColorsRepresentation&) = delete;
    HtmlExportColorsRepresentation &operator=(const HtmlExportColorsRepresentation&&) = delete;
    virtual ~HtmlExportColorsRepresentation() {}

    virtual std::string& getHtmlTextColor() { static std::string TEXT_COLOR = std::string{"#000000"}; return TEXT_COLOR; }
    virtual std::string& getHtmlBackgroundColor() { static std::string BACKGROUND_COLOR = std::string{"#FFFFFF"}; return BACKGROUND_COLOR; }
};

/**
 * @brief HTML Outline representation.
 *
 * Discount-based HTML Outline representation.
 */
class HtmlOutlineRepresentation
{
private:
    // Performance hints:
    //  - += is ~2x faster than append() (depends on cpp lib implementation)
    //  - pre-allocation of the string using reserver() is critical to avoid slow re-allocations
    //  - IMPROVE consider caching of re-used HTML snippets

    Configuration& config;
    HtmlExportColorsRepresentation exportColors;
    HtmlColorsRepresentation& lf;    
    MarkdownOutlineRepresentation markdownRepresentation;
    MarkdownTranscoder* markdownTranscoder;

    /**
     * @brief MindForger MD 2 other representation options.
     *
     * If MF options don't change, then it doesn't have to be recalculated.
     */
    unsigned int lastMfOptions;

public:
    /**
     * @brief Html O representation.
     *
     * Representation interceptor is the head of the interceptor chain.
     */
    explicit HtmlOutlineRepresentation(
            Ontology& ontology,
            HtmlColorsRepresentation& lf,
            RepresentationInterceptor* descriptionInterceptor);
    explicit HtmlOutlineRepresentation(
            Ontology& ontology,
            RepresentationInterceptor* descriptionInterceptor);
    HtmlOutlineRepresentation(const HtmlOutlineRepresentation&) = delete;
    HtmlOutlineRepresentation(const HtmlOutlineRepresentation&&) = delete;
    HtmlOutlineRepresentation &operator=(const HtmlOutlineRepresentation&) = delete;
    HtmlOutlineRepresentation &operator=(const HtmlOutlineRepresentation&&) = delete;
    virtual ~HtmlOutlineRepresentation();

    std::string* to(
        const std::string* markdown,
        std::string* html,
        std::string* basePath=nullptr,
        bool standalone=false);

    /**
     * @brief Export Outline to HTML.
     *
     * Outline is rendered to be shown in MF, standalone option indicates HTML export.
     */
    std::string* to(const Outline* outline, std::string* html, bool standalone=false);
    std::string* toHeader(Outline* outline, std::string* html, bool standalone=false, bool autolinking=false);
    std::string* to(const Note* note, std::string* html, bool autolinking=false);

    /**
     * @brief Append "color: 0x...; background-color: 0x...;"
     */
    void fgBgTextColorStyle(std::string& html);
    void outlineTypeToHtml(const OutlineType* outlineType, std::string& html);
    void noteTypeToHtml(const NoteType* noteType, std::string& html);
    void tagsToHtml(const std::vector<const Tag*>* tags, std::string& html);
    void outlineMetadataToHtml(const Outline* outline, std::string& html);

    MarkdownOutlineRepresentation& getMarkdownRepresentation() { return markdownRepresentation; }

private:
    void header(std::string& html, std::string* basePath, bool standalone);
    void footer(std::string& html);
};

} // m8r namespace

#endif /* M8R_HTML_OUTLINE_REPRESENTATION_H_ */
