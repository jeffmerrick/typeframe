import { themes as prismThemes } from "prism-react-renderer";
import type { Config } from "@docusaurus/types";
import type * as Preset from "@docusaurus/preset-classic";

// This runs in Node.js - Don't use client-side code here (browser APIs, JSX...)

const config: Config = {
  title: "Typeframe",
  tagline: "An open source portable computer",
  favicon: "img/favicon.ico",

  // Future flags, see https://docusaurus.io/docs/api/docusaurus-config#future
  future: {
    v4: true, // Improve compatibility with the upcoming Docusaurus v4
  },

  // Set the production url of your site here
  url: "https://www.typeframe.net",
  // Set the /<baseUrl>/ pathname under which your site is served
  // For GitHub pages deployment, it is often '/<projectName>/'
  baseUrl: "/",

  // GitHub pages deployment config.
  // If you aren't using GitHub pages, you don't need these.
  organizationName: "jeffmerrick", // Usually your GitHub org/user name.
  projectName: "typeframe", // Usually your repo name.

  onBrokenLinks: "throw",

  // Even if you don't use internationalization, you can use this field to set
  // useful metadata like html lang. For example, if your site is Chinese, you
  // may want to replace "en" with "zh-Hans".
  i18n: {
    defaultLocale: "en",
    locales: ["en"],
  },

  presets: [
    [
      "classic",
      {
        docs: {
          sidebarPath: "./sidebars.ts",
          // Please change this to your repo.
        },

        theme: {
          customCss: "./src/css/custom.css",
        },
      } satisfies Preset.Options,
    ],
  ],

  plugins: [
    [
      "@docusaurus/plugin-ideal-image",
      {
        quality: 100,
        min: 320,
        max: 2170,
        steps: 7,
        disableInDev: false,
      },
    ],
  ],

  themeConfig: {
    // Replace with your project's social card
    image: "img/typeframe-social-card.jpg",
    colorMode: {
      respectPrefersColorScheme: true,
    },
    navbar: {
      logo: {
        alt: "Typeframe Logo",
        src: "img/logo-light.svg",
        srcDark: "img/logo-dark.svg",
      },
      items: [
        {
          type: "docSidebar",
          sidebarId: "docsSidebar",
          position: "right",
          label: "Build Guide",
        },
        {
          to: "/gallery",
          label: "Gallery",
          position: "right",
        },
        {
          href: "https://github.com/jeffmerrick/typeframe",
          label: "GitHub",
          position: "right",
        },
      ],
    },
    footer: {
      style: "light",
      links: [
        {
          title: "Docs",
          items: [
            {
              label: "PX-88 Build Guide",
              to: "/docs/px-88",
            },
            {
              label: "PS-85 (Coming Soon)",
              to: "/docs/ps-85",
            },
          ],
        },
        {
          title: "Other Links",
          items: [
            {
              label: "GitHub",
              href: "https://github.com/jeffmerrick/typeframe",
            },

            {
              label: "Instagram",
              href: "https://www.instagram.com/jeffsbuilds/",
            },
          ],
        },
      ],
      copyright: `<span>A project by <a href="https://www.instagram.com/jeffsbuilds" target="_blank" rel="noopener noreferrer">Jeff</a></span> <a href="/license">Licenses</a>`,
    },
    prism: {
      theme: prismThemes.gruvboxMaterialDark,
      darkTheme: prismThemes.gruvboxMaterialDark,
    },
  } satisfies Preset.ThemeConfig,
};

export default config;
